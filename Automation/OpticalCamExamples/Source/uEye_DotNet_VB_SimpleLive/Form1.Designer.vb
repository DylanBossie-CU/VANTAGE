<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class uEye_DotNet_Simple_Live
    Inherits System.Windows.Forms.Form

    'Das Formular überschreibt den Löschvorgang, um die Komponentenliste zu bereinigen.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Wird vom Windows Form-Designer benötigt.
    Private components As System.ComponentModel.IContainer

    'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
    'Das Bearbeiten ist mit dem Windows Form-Designer möglich.  
    'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(uEye_DotNet_Simple_Live))
        Me.Button_Exit_Prog = New System.Windows.Forms.Button
        Me.CB_Auto_Gain_Balance = New System.Windows.Forms.CheckBox
        Me.CB_Auto_White_Balance = New System.Windows.Forms.CheckBox
        Me.Button_Load_Parameter = New System.Windows.Forms.Button
        Me.Button_Freeze_Video = New System.Windows.Forms.Button
        Me.Button_Stop_Video = New System.Windows.Forms.Button
        Me.Button_Live_Video = New System.Windows.Forms.Button
        Me.DisplayWindow = New System.Windows.Forms.PictureBox
        CType(Me.DisplayWindow, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Button_Exit_Prog
        '
        Me.Button_Exit_Prog.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Exit_Prog.Location = New System.Drawing.Point(506, 345)
        Me.Button_Exit_Prog.Name = "Button_Exit_Prog"
        Me.Button_Exit_Prog.Size = New System.Drawing.Size(117, 29)
        Me.Button_Exit_Prog.TabIndex = 15
        Me.Button_Exit_Prog.Text = "Exit"
        Me.Button_Exit_Prog.UseVisualStyleBackColor = True
        '
        'CB_Auto_Gain_Balance
        '
        Me.CB_Auto_Gain_Balance.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CB_Auto_Gain_Balance.AutoSize = True
        Me.CB_Auto_Gain_Balance.Location = New System.Drawing.Point(506, 198)
        Me.CB_Auto_Gain_Balance.Name = "CB_Auto_Gain_Balance"
        Me.CB_Auto_Gain_Balance.Size = New System.Drawing.Size(115, 17)
        Me.CB_Auto_Gain_Balance.TabIndex = 14
        Me.CB_Auto_Gain_Balance.Text = "Auto Gain Balance"
        Me.CB_Auto_Gain_Balance.UseVisualStyleBackColor = True
        '
        'CB_Auto_White_Balance
        '
        Me.CB_Auto_White_Balance.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CB_Auto_White_Balance.AutoSize = True
        Me.CB_Auto_White_Balance.Location = New System.Drawing.Point(506, 174)
        Me.CB_Auto_White_Balance.Name = "CB_Auto_White_Balance"
        Me.CB_Auto_White_Balance.Size = New System.Drawing.Size(121, 17)
        Me.CB_Auto_White_Balance.TabIndex = 13
        Me.CB_Auto_White_Balance.Text = "Auto White Balance"
        Me.CB_Auto_White_Balance.UseVisualStyleBackColor = True
        '
        'Button_Load_Parameter
        '
        Me.Button_Load_Parameter.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Load_Parameter.Location = New System.Drawing.Point(506, 117)
        Me.Button_Load_Parameter.Name = "Button_Load_Parameter"
        Me.Button_Load_Parameter.Size = New System.Drawing.Size(117, 29)
        Me.Button_Load_Parameter.TabIndex = 12
        Me.Button_Load_Parameter.Text = "Load Parameter"
        Me.Button_Load_Parameter.UseVisualStyleBackColor = True
        '
        'Button_Freeze_Video
        '
        Me.Button_Freeze_Video.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Freeze_Video.Location = New System.Drawing.Point(506, 82)
        Me.Button_Freeze_Video.Name = "Button_Freeze_Video"
        Me.Button_Freeze_Video.Size = New System.Drawing.Size(117, 29)
        Me.Button_Freeze_Video.TabIndex = 11
        Me.Button_Freeze_Video.Text = "Freeze Video"
        Me.Button_Freeze_Video.UseVisualStyleBackColor = True
        '
        'Button_Stop_Video
        '
        Me.Button_Stop_Video.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Stop_Video.Location = New System.Drawing.Point(506, 47)
        Me.Button_Stop_Video.Name = "Button_Stop_Video"
        Me.Button_Stop_Video.Size = New System.Drawing.Size(117, 29)
        Me.Button_Stop_Video.TabIndex = 10
        Me.Button_Stop_Video.Text = "Stop Live"
        Me.Button_Stop_Video.UseVisualStyleBackColor = True
        '
        'Button_Live_Video
        '
        Me.Button_Live_Video.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Live_Video.Location = New System.Drawing.Point(506, 12)
        Me.Button_Live_Video.Name = "Button_Live_Video"
        Me.Button_Live_Video.Size = New System.Drawing.Size(117, 29)
        Me.Button_Live_Video.TabIndex = 9
        Me.Button_Live_Video.Text = "Start Live"
        Me.Button_Live_Video.UseVisualStyleBackColor = True
        '
        'DisplayWindow
        '
        Me.DisplayWindow.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DisplayWindow.Image = CType(resources.GetObject("DisplayWindow.Image"), System.Drawing.Image)
        Me.DisplayWindow.Location = New System.Drawing.Point(13, 12)
        Me.DisplayWindow.Name = "DisplayWindow"
        Me.DisplayWindow.Size = New System.Drawing.Size(468, 362)
        Me.DisplayWindow.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage
        Me.DisplayWindow.TabIndex = 8
        Me.DisplayWindow.TabStop = False
        '
        'uEye_DotNet_Simple_Live
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(641, 386)
        Me.Controls.Add(Me.Button_Exit_Prog)
        Me.Controls.Add(Me.CB_Auto_Gain_Balance)
        Me.Controls.Add(Me.CB_Auto_White_Balance)
        Me.Controls.Add(Me.Button_Load_Parameter)
        Me.Controls.Add(Me.Button_Freeze_Video)
        Me.Controls.Add(Me.Button_Stop_Video)
        Me.Controls.Add(Me.Button_Live_Video)
        Me.Controls.Add(Me.DisplayWindow)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MinimumSize = New System.Drawing.Size(649, 420)
        Me.Name = "uEye_DotNet_Simple_Live"
        Me.Text = "uEye VB Simple Live"
        CType(Me.DisplayWindow, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Button_Exit_Prog As System.Windows.Forms.Button
    Friend WithEvents CB_Auto_Gain_Balance As System.Windows.Forms.CheckBox
    Friend WithEvents CB_Auto_White_Balance As System.Windows.Forms.CheckBox
    Friend WithEvents Button_Load_Parameter As System.Windows.Forms.Button
    Friend WithEvents Button_Freeze_Video As System.Windows.Forms.Button
    Friend WithEvents Button_Stop_Video As System.Windows.Forms.Button
    Friend WithEvents Button_Live_Video As System.Windows.Forms.Button
    Friend WithEvents DisplayWindow As System.Windows.Forms.PictureBox

End Class
