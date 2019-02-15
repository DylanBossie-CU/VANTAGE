Public Class uEye_DotNet_Simple_Live

    Dim WithEvents camera As New uEye.Camera()
    Dim DisplayHandle As IntPtr
    Dim bLive As Boolean
    Dim nNumberOfSeqBuffers As Int32 = 3


    Private Sub uEye_DotNet_Simple_Live_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        DisplayHandle = DisplayWindow.Handle
        CameraInit()
    End Sub


    Private Sub EventHandler() Handles camera.EventFrame
        'Event
        Dim statusRet As uEye.Defines.Status
        Dim MemId As Int32

        statusRet = camera.Memory.GetLast(MemId)

        If ((uEye.Defines.Status.Success = statusRet) And (0 < MemId)) Then
            statusRet = camera.Memory.Lock(MemId)

            If (uEye.Defines.Status.Success = statusRet) Then
                camera.Display.Render(MemId, DisplayHandle, uEye.Defines.DisplayRenderMode.FitToWindow)
                camera.Memory.Unlock(MemId)
            End If
        End If
    End Sub


    Private Function AllocImageMems() As uEye.Defines.Status
        Dim statusRet As uEye.Defines.Status

        For i As Int32 = 1 To nNumberOfSeqBuffers
            statusRet = camera.Memory.Allocate()

            If (statusRet <> uEye.Defines.Status.Success) Then
                FreeImageMems()
            End If
        Next

        Return statusRet
    End Function


    Private Function FreeImageMems() As uEye.Defines.Status
        Dim statusRet As uEye.Defines.Status
        Dim idList As Int32() = {}

        statusRet = camera.Memory.GetList(idList)

        If (uEye.Defines.Status.Success = statusRet) Then
            For Each nMemID As Int32 In idList
                Do While True
                    statusRet = camera.Memory.Free(nMemID)

                    If (uEye.Defines.Status.SEQ_BUFFER_IS_LOCKED = statusRet) Then
                        System.Threading.Thread.Sleep(1)
                        Continue Do
                    End If

                    Exit Do
                Loop
            Next
        End If

        Return statusRet
    End Function


    Private Function InitSequence() As uEye.Defines.Status
        Dim statusRet As uEye.Defines.Status
        Dim idList As Int32() = {}

        statusRet = camera.Memory.GetList(idList)

        If (uEye.Defines.Status.Success = statusRet) Then
            statusRet = camera.Memory.Sequence.Add(idList)

            If (uEye.Defines.Status.Success <> statusRet) Then
                ClearSequence()
            End If
        End If

        Return statusRet
    End Function


    Private Function ClearSequence() As uEye.Defines.Status
        Return camera.Memory.Sequence.Clear()
    End Function


    Private Sub CameraInit()
        Dim statusRet As uEye.Defines.Status

        'Open Camera
        statusRet = camera.Init()
        If (statusRet <> uEye.Defines.Status.Success) Then
            MessageBox.Show("Camera initializing failed")
        End If

        'Allocate Memory
        statusRet = AllocImageMems()
        If (statusRet <> uEye.Defines.Status.Success) Then
            MessageBox.Show("Allocate Memory failed")
        End If

        statusRet = InitSequence()
        If (statusRet <> uEye.Defines.Status.Success) Then
            MessageBox.Show("Add to sequence failed")
        End If

        'Start Live
        statusRet = camera.Acquisition.Capture()
        If (statusRet <> uEye.Defines.Status.Success) Then
            MessageBox.Show("Start Live Video failed")
        Else
            bLive = True
        End If

        CB_Auto_Gain_Balance.Enabled = camera.AutoFeatures.Software.Gain.Supported
        CB_Auto_White_Balance.Enabled = camera.AutoFeatures.Software.WhiteBalance.Supported
    End Sub


    Private Sub Button_Live_Video_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Live_Video.Click
        'Start Live Video
        If (camera.Acquisition.Capture() = uEye.Defines.Status.Success) Then
            bLive = True
        End If
    End Sub


    Private Sub Button_Stop_Video_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Stop_Video.Click
        'Stop Live Video
        If (camera.Acquisition.Stop() = uEye.Defines.Status.Success) Then
            bLive = False
        End If
    End Sub


    Private Sub Button_Freeze_Video_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Freeze_Video.Click
        'Freeze Video
        If (camera.Acquisition.Freeze() = uEye.Defines.Status.Success) Then
            bLive = False
        End If
    End Sub


    Private Sub Button_Load_Parameter_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Load_Parameter.Click
        'Load Parameters
        camera.Acquisition.Stop()

        ClearSequence()
        FreeImageMems()

        camera.Parameter.Load("")

        AllocImageMems()
        InitSequence()

        If (bLive = True) Then
            If (camera.Acquisition.Capture() = uEye.Defines.Status.Success) Then
                bLive = True
            End If
        End If
    End Sub

    Private Sub CB_Auto_White_Balance_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CB_Auto_White_Balance.CheckedChanged
        'CheckBox Auto White Balance
        camera.AutoFeatures.Software.WhiteBalance.SetEnable(CB_Auto_White_Balance.Checked)
    End Sub


    Private Sub CB_Auto_Gain_Balance_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CB_Auto_Gain_Balance.CheckedChanged
        'CheckBox Auto Gain Balance
        camera.AutoFeatures.Software.Gain.SetEnable(CB_Auto_Gain_Balance.Checked)
    End Sub


    Private Sub Button_Exit_Prog_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Exit_Prog.Click
        camera.Acquisition.Stop()

        ClearSequence()
        FreeImageMems()

        'Close the Camera
        camera.Exit()

        'Program Exit
        Close()
    End Sub
End Class
