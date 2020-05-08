# $language = "python"
# $interface = "1.0"

import os
import SecureCRT

def xmodem(filename):
	with open(filename, 'r') as f:
		content = f.read()

	# XXX - SecureCRT pads with chr(26)
	content += chr(26) * (128 - (len(content)%128))

	crt.Screen.WaitForString(chr(21)+chr(21)+chr(21)) #NAK x 3

	for i in range(len(content)/128):
		packet = content[(i*128):((i+1)*128)]

		crt.Screen.Send(chr(1))	  #SOH
		crt.Screen.Send(chr(127)) #XXX - Fake block number
		                          #      SecureCRT can only
		                          #      send ASCII chars???
		crt.Screen.Send(chr(127)) #XXX - Fake block number complement
		crt.Screen.Send(packet)
		crt.Screen.Send(chr(127)) #XXX - Fake checksum

		idx = crt.Screen.WaitForStrings([chr(6), chr(21)], 1) #[ACK, NAK]
		if idx == 0: # Time out
			crt.Dialog.MessageBox("Error.\n" + "Timeout.")
			return
		elif idx == 1: #ACK
			pass
		elif idx == 2: #NAK
			crt.Dialog.MessageBox("Error.\n" + "Canceled.")
			return

	crt.Screen.Send(chr(4)) #EOT
	crt.Screen.WaitForString(chr(6), 1) #ACK

def Main():
	tab = crt.GetScriptTab()

	if tab.Session.Connected != True:
		crt.Dialog.MessageBox("Error.\n" + "No connection.")
		return

	crt.Screen.Synchronous = True

 	aout_uue = '/path/to/a.out.uue'
 	kernel_uue = '/path/to/kernel.img.uue'

	if os.path.getmtime(aout_uue) > os.path.getmtime(kernel_uue):
		xmodem(aout_uue)
		crt.Sleep(100)
	xmodem(kernel_uue)

Main()
