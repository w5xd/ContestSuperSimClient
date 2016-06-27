/* I hope nobody really wants to call the Contest Super Simulator 
** from the Windows Scripting Host...but this is to show that it
** can be done.
**
** Scripting can't seem to directly call Win32 GetActiveObject
** so use ContestSuperSimulator.ActiveSimulator to get it.
** GetActiveObject requires no admin or installation. But the
** ScriptHelper, by contrast, must be installed for WScript.CreateObject 
** to find it.
** 
** If this create object fails, check to see if the Contest Super Simulator's
** ScriptHelper.dll is registered, and the one for the correct
** OS.  (64b versus 32b). No harm registering the "other" one, but
** no help, either.
*/
var helper = WScript.CreateObject("ContestSuperSimulator.ActiveSimulator");
var manager = helper.Current;
var radio = manager.GetSimulatorForRadio(0); // zero-based radio number

WScript.Echo("Center =" + radio.CenterFrequency);

var notifications = "\n";

// This function object can be called back by GetCwSidetoneGenerator
function CwSidetoneNotify() {
    // These must be the first two declarations
    this.CwQueued = function (id) { // DISPID = 1
        notifications += "Started " + id + "\n";
    };
    // ... so that their DISPID's are 1 and 2, respectively
    this.CwFinished = function (id, msg) { // DISPID = 2
        notifications += "Finished " + id + " " + msg + "\n";
    };
}

var Cw = manager.GetCwSidetoneGenerator(new CwSidetoneNotify);
Cw.WPM = 25;
Cw.QueueToTransmitText("test de w5xd");

WScript.Echo("CW in progress");
WScript.Echo("Result " + notifications);