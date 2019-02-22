import o3d3xx
import sys
import time

if len(sys.argv) > 1:
	address=sys.argv[1]
else:
	address='169.254.145.24'

# create device
device = o3d3xx.Device(address)

# open a session and create an application for editing
session = device.requestSession()
session.startEdit()
applicationIndex = session.edit.createApplication()
application = session.edit.editApplication(applicationIndex)

# configure the application to
# - double exposure
application.imagerConfig.changeType("under5m_moderate")
# - free-run at given fps
application.setParameter("TriggerMode", "1")
application.imagerConfig.setParameter("FrameRate", "10")
application.imagerConfig.setParameter("Resolution","1") #mode 1 uses the full resolution of our camera, muy neccessary
#print(a.value)
# and perform an auto-exposure run to determine
# exposure times
application.imagerConfig.startCalculateExposureTime()
# wait until the auto-exposure process has finished
while application.imagerConfig.getProgressCalculateExposureTime() < 1.0:
	time.sleep(1)
# name and save the application and stop editing
application.setParameter("Name", "o3d3xx-python example application")
application.save()
session.edit.stopEditingApplication()

# set the new application as active and save the change
session.edit.device.setParameter("ActiveApplication", str(applicationIndex))
session.edit.device.save()

# finish the session
session.cancelSession()
