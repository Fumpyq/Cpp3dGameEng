; Listing generated by Microsoft (R) Optimizing Compiler Version 19.36.32538.0 

include listing.inc

INCLUDELIB OLDNAMES

PUBLIC	??_C@_0M@BNJJKMGF@?$GA?9?$DN?$FL?$FN?2?0?$DL?8?4?1@ ; `string'
PUBLIC	??_C@_0BA@LCLOGKDG@imgui_impl_glfw@		; `string'
EXTRN	glfwSetCharCallback:PROC
EXTRN	glfwSetErrorCallback:PROC
EXTRN	glfwSetWindowFocusCallback:PROC
EXTRN	glfwGetCursorPos:PROC
EXTRN	glfwGetKeyName:PROC
EXTRN	glfwSetCursorPosCallback:PROC
EXTRN	glfwGetGamepadState:PROC
EXTRN	glfwGetWin32Window:PROC
EXTRN	glfwSetCursorPos:PROC
EXTRN	glfwGetError:PROC
EXTRN	glfwCreateStandardCursor:PROC
EXTRN	glfwSetInputMode:PROC
EXTRN	glfwSetScrollCallback:PROC
EXTRN	glfwSetKeyCallback:PROC
EXTRN	glfwDestroyCursor:PROC
EXTRN	glfwSetCursor:PROC
EXTRN	glfwSetMouseButtonCallback:PROC
EXTRN	glfwGetWindowSize:PROC
EXTRN	glfwSetCursorEnterCallback:PROC
EXTRN	glfwSetClipboardString:PROC
EXTRN	glfwGetFramebufferSize:PROC
EXTRN	glfwSetMonitorCallback:PROC
EXTRN	glfwGetTime:PROC
EXTRN	glfwGetWindowAttrib:PROC
EXTRN	glfwGetInputMode:PROC
EXTRN	glfwGetClipboardString:PROC
;	COMDAT ??_C@_0BA@LCLOGKDG@imgui_impl_glfw@
CONST	SEGMENT
??_C@_0BA@LCLOGKDG@imgui_impl_glfw@ DB 'imgui_impl_glfw', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0M@BNJJKMGF@?$GA?9?$DN?$FL?$FN?2?0?$DL?8?4?1@
CONST	SEGMENT
??_C@_0M@BNJJKMGF@?$GA?9?$DN?$FL?$FN?2?0?$DL?8?4?1@ DB '`-=[]\,;''./', 00H ; `string'
PUBLIC	??$IM_DELETE@UImGui_ImplGlfw_Data@@@@YAXPEAUImGui_ImplGlfw_Data@@@Z ; IM_DELETE<ImGui_ImplGlfw_Data>
PUBLIC	?ImGui_ImplGlfw_NewFrame@@YAXXZ			; ImGui_ImplGlfw_NewFrame
PUBLIC	?ImGui_ImplGlfw_Shutdown@@YAXXZ			; ImGui_ImplGlfw_Shutdown
PUBLIC	?ImGui_ImplGlfw_InitForOpenGL@@YA_NPEAUGLFWwindow@@_N@Z ; ImGui_ImplGlfw_InitForOpenGL
PUBLIC	?ImGui_ImplGlfw_RestoreCallbacks@@YAXPEAUGLFWwindow@@@Z ; ImGui_ImplGlfw_RestoreCallbacks
PUBLIC	?ImGui_ImplGlfw_InstallCallbacks@@YAXPEAUGLFWwindow@@@Z ; ImGui_ImplGlfw_InstallCallbacks
PUBLIC	?ImGui_ImplGlfw_MonitorCallback@@YAXPEAUGLFWmonitor@@H@Z ; ImGui_ImplGlfw_MonitorCallback
PUBLIC	?ImGui_ImplGlfw_CharCallback@@YAXPEAUGLFWwindow@@I@Z ; ImGui_ImplGlfw_CharCallback
PUBLIC	?ImGui_ImplGlfw_CursorEnterCallback@@YAXPEAUGLFWwindow@@H@Z ; ImGui_ImplGlfw_CursorEnterCallback
PUBLIC	?ImGui_ImplGlfw_CursorPosCallback@@YAXPEAUGLFWwindow@@NN@Z ; ImGui_ImplGlfw_CursorPosCallback
PUBLIC	?ImGui_ImplGlfw_WindowFocusCallback@@YAXPEAUGLFWwindow@@H@Z ; ImGui_ImplGlfw_WindowFocusCallback
PUBLIC	?ImGui_ImplGlfw_KeyCallback@@YAXPEAUGLFWwindow@@HHHH@Z ; ImGui_ImplGlfw_KeyCallback
PUBLIC	?ImGui_ImplGlfw_ScrollCallback@@YAXPEAUGLFWwindow@@NN@Z ; ImGui_ImplGlfw_ScrollCallback
PUBLIC	?ImGui_ImplGlfw_MouseButtonCallback@@YAXPEAUGLFWwindow@@HHH@Z ; ImGui_ImplGlfw_MouseButtonCallback
PUBLIC	??0ImGui_ImplGlfw_Data@@QEAA@XZ			; ImGui_ImplGlfw_Data::ImGui_ImplGlfw_Data
;	COMDAT pdata
pdata	SEGMENT
$pdata$??$IM_DELETE@UImGui_ImplGlfw_Data@@@@YAXPEAUImGui_ImplGlfw_Data@@@Z DD imagerel $LN13@IM_DELETE
	DD	imagerel $LN13@IM_DELETE+38
	DD	imagerel $unwind$??$IM_DELETE@UImGui_ImplGlfw_Data@@@@YAXPEAUImGui_ImplGlfw_Data@@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_NewFrame@@YAXXZ DD imagerel $LN41@ImGui_Impl
	DD	imagerel $LN41@ImGui_Impl+592
	DD	imagerel $unwind$?ImGui_ImplGlfw_NewFrame@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_UpdateGamepads@@YAXXZ DD imagerel ?ImGui_ImplGlfw_UpdateGamepads@@YAXXZ
	DD	imagerel ?ImGui_ImplGlfw_UpdateGamepads@@YAXXZ+1072
	DD	imagerel $unwind$?ImGui_ImplGlfw_UpdateGamepads@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_UpdateMouseCursor@@YAXXZ DD imagerel ?ImGui_ImplGlfw_UpdateMouseCursor@@YAXXZ
	DD	imagerel ?ImGui_ImplGlfw_UpdateMouseCursor@@YAXXZ+144
	DD	imagerel $unwind$?ImGui_ImplGlfw_UpdateMouseCursor@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_UpdateMouseData@@YAXXZ DD imagerel ?ImGui_ImplGlfw_UpdateMouseData@@YAXXZ
	DD	imagerel ?ImGui_ImplGlfw_UpdateMouseData@@YAXXZ+282
	DD	imagerel $unwind$?ImGui_ImplGlfw_UpdateMouseData@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_Shutdown@@YAXXZ DD imagerel $LN27@ImGui_Impl
	DD	imagerel $LN27@ImGui_Impl+146
	DD	imagerel $unwind$?ImGui_ImplGlfw_Shutdown@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_Init@@YA_NPEAUGLFWwindow@@_NW4GlfwClientApi@@@Z DD imagerel ?ImGui_ImplGlfw_Init@@YA_NPEAUGLFWwindow@@_NW4GlfwClientApi@@@Z
	DD	imagerel ?ImGui_ImplGlfw_Init@@YA_NPEAUGLFWwindow@@_NW4GlfwClientApi@@@Z+341
	DD	imagerel $unwind$?ImGui_ImplGlfw_Init@@YA_NPEAUGLFWwindow@@_NW4GlfwClientApi@@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_RestoreCallbacks@@YAXPEAUGLFWwindow@@@Z DD imagerel $LN8@ImGui_Impl
	DD	imagerel $LN8@ImGui_Impl+219
	DD	imagerel $unwind$?ImGui_ImplGlfw_RestoreCallbacks@@YAXPEAUGLFWwindow@@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_InstallCallbacks@@YAXPEAUGLFWwindow@@@Z DD imagerel $LN8@ImGui_Impl
	DD	imagerel $LN8@ImGui_Impl+216
	DD	imagerel $unwind$?ImGui_ImplGlfw_InstallCallbacks@@YAXPEAUGLFWwindow@@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_CharCallback@@YAXPEAUGLFWwindow@@I@Z DD imagerel $LN9@ImGui_Impl
	DD	imagerel $LN9@ImGui_Impl+80
	DD	imagerel $unwind$?ImGui_ImplGlfw_CharCallback@@YAXPEAUGLFWwindow@@I@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_CursorEnterCallback@@YAXPEAUGLFWwindow@@H@Z DD imagerel $LN14@ImGui_Impl
	DD	imagerel $LN14@ImGui_Impl+165
	DD	imagerel $unwind$?ImGui_ImplGlfw_CursorEnterCallback@@YAXPEAUGLFWwindow@@H@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_CursorPosCallback@@YAXPEAUGLFWwindow@@NN@Z DD imagerel $LN12@ImGui_Impl
	DD	imagerel $LN12@ImGui_Impl+167
	DD	imagerel $unwind$?ImGui_ImplGlfw_CursorPosCallback@@YAXPEAUGLFWwindow@@NN@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_WindowFocusCallback@@YAXPEAUGLFWwindow@@H@Z DD imagerel $LN23@ImGui_Impl
	DD	imagerel $LN23@ImGui_Impl+228
	DD	imagerel $unwind$?ImGui_ImplGlfw_WindowFocusCallback@@YAXPEAUGLFWwindow@@H@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_KeyCallback@@YAXPEAUGLFWwindow@@HHHH@Z DD imagerel $LN36@ImGui_Impl
	DD	imagerel $LN36@ImGui_Impl+270
	DD	imagerel $unwind$?ImGui_ImplGlfw_KeyCallback@@YAXPEAUGLFWwindow@@HHHH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_TranslateUntranslatedKey@@YAHHH@Z DD imagerel ?ImGui_ImplGlfw_TranslateUntranslatedKey@@YAHHH@Z
	DD	imagerel ?ImGui_ImplGlfw_TranslateUntranslatedKey@@YAHHH@Z+280
	DD	imagerel $unwind$?ImGui_ImplGlfw_TranslateUntranslatedKey@@YAHHH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_ScrollCallback@@YAXPEAUGLFWwindow@@NN@Z DD imagerel $LN22@ImGui_Impl
	DD	imagerel $LN22@ImGui_Impl+253
	DD	imagerel $unwind$?ImGui_ImplGlfw_ScrollCallback@@YAXPEAUGLFWwindow@@NN@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_MouseButtonCallback@@YAXPEAUGLFWwindow@@HHH@Z DD imagerel $LN10@ImGui_Impl
	DD	imagerel $LN10@ImGui_Impl+115
	DD	imagerel $unwind$?ImGui_ImplGlfw_MouseButtonCallback@@YAXPEAUGLFWwindow@@HHH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?ImGui_ImplGlfw_UpdateKeyModifiers@@YAXH@Z DD imagerel ?ImGui_ImplGlfw_UpdateKeyModifiers@@YAXH@Z
	DD	imagerel ?ImGui_ImplGlfw_UpdateKeyModifiers@@YAXH@Z+123
	DD	imagerel $unwind$?ImGui_ImplGlfw_UpdateKeyModifiers@@YAXH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??0ImGui_ImplGlfw_Data@@QEAA@XZ DD imagerel $LN6@ImGui_Impl
	DD	imagerel $LN6@ImGui_Impl+35
	DD	imagerel $unwind$??0ImGui_ImplGlfw_Data@@QEAA@XZ
;	COMDAT xdata
xdata	SEGMENT
$unwind$??0ImGui_ImplGlfw_Data@@QEAA@XZ DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_UpdateKeyModifiers@@YAXH@Z DQ 00006340a00040a01r ; 8.62672e-309
	DD	07006320aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_MouseButtonCallback@@YAXPEAUGLFWwindow@@HHH@Z DQ 00007640f00060f01r ; 1.02782e-308
	DQ	0700b320f0006340fr		; 5.2777e+231
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_ScrollCallback@@YAXPEAUGLFWwindow@@NN@Z DQ 00004781a00061a01r ; 6.21511e-309
	DQ	03002b20600056812r		; 2.01821e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_TranslateUntranslatedKey@@YAHHH@Z DQ 00013640f00061e19r ; 2.69663e-308
	DQ	0700bd20f0012340fr		; 5.39899e+231
	DD	imagerel __GSHandlerCheck
	DD	060H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_KeyCallback@@YAXPEAUGLFWwindow@@HHHH@Z DQ 0000a641400081401r ; 1.44504e-308
	DQ	00008341400095414r		; 1.14083e-308
	DD	070105214H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_WindowFocusCallback@@YAXPEAUGLFWwindow@@H@Z DQ 0000a340a00040a01r ; 1.41894e-308
	DD	07006720aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_CursorPosCallback@@YAXPEAUGLFWwindow@@NN@Z DQ 000028826000a2601r ; 3.52094e-309
	DQ	00004681a00037821r		; 6.1282e-309
	DQ	07008920c000c340cr		; 4.76827e+231
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_CursorEnterCallback@@YAXPEAUGLFWwindow@@H@Z DQ 00007640f00060f01r ; 1.02782e-308
	DQ	0700b320f0006340fr		; 5.2777e+231
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_CharCallback@@YAXPEAUGLFWwindow@@I@Z DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_InstallCallbacks@@YAXPEAUGLFWwindow@@@Z DQ 00006340a00040a01r ; 8.62672e-309
	DD	07006320aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_RestoreCallbacks@@YAXPEAUGLFWwindow@@@Z DQ 00006340a00040a01r ; 8.62672e-309
	DD	07006320aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_Init@@YA_NPEAUGLFWwindow@@_NW4GlfwClientApi@@@Z DQ 0000a340c00060c01r ; 1.41894e-308
	DQ	0500660077008320cr		; 3.23858e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_Shutdown@@YAXXZ DQ 00008641400081401r ; 1.1669e-308
	DQ	00006341400075414r		; 8.62693e-309
	DD	070103214H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_UpdateMouseData@@YAXXZ DQ 0000b640f00061e19r ; 1.58409e-308
	DQ	0700b720f000a340fr		; 5.32621e+231
	DD	imagerel __GSHandlerCheck
	DD	030H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_UpdateMouseCursor@@YAXXZ DQ 00006340a00040a01r ; 8.62672e-309
	DD	07006320aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_UpdateGamepads@@YAXXZ DQ 00005a82e00113c19r ; 7.86696e-309
	DQ	00007882400069829r		; 1.04743e-308
	DQ	00009681b0008781fr		; 1.30816e-308
	DQ	00016341700177417r		; 3.08777e-308
	DQ	00000500c00140117r		; 4.34839e-310
	DD	imagerel __GSHandlerCheck
	DD	048H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?ImGui_ImplGlfw_NewFrame@@YAXXZ DQ 0000e741700082519r ; 2.01e-308
	DQ	0000c3417000d6417r		; 1.6971e-308
	DD	050109217H
	DD	imagerel __GSHandlerCheck
	DD	040H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??$IM_DELETE@UImGui_ImplGlfw_Data@@@@YAXPEAUImGui_ImplGlfw_Data@@@Z DQ 00000420400010401r ; 3.58617e-310
END
