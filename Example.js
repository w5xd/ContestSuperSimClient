/* I hope nobody really wants to call the Contest Super Simulator 
** from the Windows Scripting Host...but this is to show that the
** simulator's interface is easy to use. All the objects do
** old, simple, late-binding COM automation.
**
** The opening is different from the C++ example because
** WScript can't seem to directly call Win32 GetActiveObject
** so use ContestSuperSimulator.ActiveSimulator to get it.
** 
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