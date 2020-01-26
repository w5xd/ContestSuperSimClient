using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Runtime.InteropServices;

// These classes wrap the OLE automation interfaces of the Contest Super Simulator
/// <summary>
/// class Radio represents one of the simulator's virtual radios, numbered zero through three
/// class CwSideToneSimulator wraps the simulator's sidetone generator that plays through a Windows sound device
/// class StationList wraps the simulator's representation of the stations it has virtualized.
/// class CSS is the manager object. Do a "new CSS()" to get started. It will fail if CSS is not currently running. 
/// class CwSidetoneNotifyExample is a template for a class you must implement in order to synchronize CW without WinKey
/// </summary>

namespace CSSclient
{
    public class Radio
    {
        private object radio;
        private Type radioType;
        public Radio(object r)
        {
            radio = r;
            radioType = radio.GetType();
        }

        public short HeadphoneMask { /* This radio's audio goes to audio channel mask. 0=off, 1=left, 2=right, 3=both*/
            set {
                radioType.InvokeMember("HeadphoneMask", BindingFlags.SetProperty, null, radio, new object[1] { value });
            }
        }
       
        //Logger can tell the simulator the virtual IF center.
        public double CenterFrequency  /* KHz*/ {
            get {
                return (double)radioType.InvokeMember("CenterFrequency", BindingFlags.GetProperty, null, radio, null);
            }
            set {
                radioType.InvokeMember("CenterFrequency", BindingFlags.SetProperty, null, radio, new object[1] { value });
                }
        }

        // Logger can tell the simulator to tune. Accepted Modes are CW PH RY
        public void SetListenFrequency(double f, string mode)
        {
            radioType.InvokeMember("SetListenFrequency", BindingFlags.InvokeMethod, null, radio, new object[2] { f, mode });
        }
        // am transmitting now (not listening) on this radio
        public void MessageStartedNow()
        {
            radioType.InvokeMember("MessageStartedNow", BindingFlags.InvokeMethod, null, radio, null);
        }
        // Logger tells simulator what was transmitted and when it finished
        public void MessageCompletedNow(string mode, string messageText)
        {
            radioType.InvokeMember("MessageCompletedNow", BindingFlags.InvokeMethod, null, radio, new object[2] { mode, messageText });
        }
        // Logger tells simulator this radio has TX focus. Implied by MessageStartedNow
        public void HaveTransmitFocus()
        {
            radioType.InvokeMember("HaveTransmitFocus", BindingFlags.InvokeMethod, null, radio, null);
        }
        //List of active stations 
        public StationList SimulatedStations {
            get {
                return new StationList(radioType.InvokeMember("SimulatedStations", BindingFlags.InvokeMethod, null, radio, null));
            }
        }

        // Bandwidth, in kHz, of the simulation
        public double Bandwidth { get { return (double)radioType.InvokeMember("Bandwidth", BindingFlags.GetProperty, null, radio, null); } }
    }

    public class CwSideToneSimulator
    {
        private object cw;
        private Type cwType;
        public CwSideToneSimulator(object o)
        {
            cw = o;
            cwType = cw.GetType();
        }

        public void QueueToTransmitText(string CwMessage)
        {
            cwType.InvokeMember("QueueToTransmitText", BindingFlags.InvokeMethod, null, cw, new object[1] { CwMessage });
        }

        public void QueueWpmChange(short wpm)
        {
            cwType.InvokeMember("QueueWpmChange", BindingFlags.InvokeMethod, null, cw, new object[1] { wpm });
        }

        public short WPM {
            set {
                cwType.InvokeMember("WPM", BindingFlags.SetProperty, null, cw, new object[1] { value });
            }
        }

        public void AbortTransmission()
        {
            cwType.InvokeMember("AbortTransmission", BindingFlags.InvokeMethod, null, cw, null);
        }

        // audio channel mask. 0=off, 1=left, 2=right, 3=both
        public short HeadphoneMask {
            set {
                cwType.InvokeMember("HeadphoneMask", BindingFlags.SetProperty, null, cw, new object[1] { value });
            }
        }
    }

    public class StationList
    {
        private object stationList;
        private Type stationListType;
        public StationList(object o)
        {
            stationList = o;
            stationListType = stationList.GetType();
        }

        public int Count {
            get {
                return (int)stationListType.InvokeMember("Count", BindingFlags.GetProperty, null, stationList, null);
            }
        }

        public string GetModulationMode(short idx)
        {
            return (string)stationListType.InvokeMember("ModulationMode", BindingFlags.GetProperty, null, stationList, new object[1] { idx });
        }

        public string GetCallsign(short idx)
        {
            return (string)stationListType.InvokeMember("Callsign", BindingFlags.GetProperty, null, stationList, new object[1] { idx });
        }

        public string GetSpOrCq(short idx)
        {
            return (string)stationListType.InvokeMember("SpOrCq", BindingFlags.GetProperty, null, stationList, new object[1] { idx });
        }

        public double GetFreqKhz(short idx)
        {
            return (double)stationListType.InvokeMember("FreqKhz", BindingFlags.GetProperty, null, stationList, new object[1] { idx });
        }
    }

    public class CSS
    {
        private object css;
        private Type cssType;
        public CSS()
        {
            Type t = Type.GetTypeFromProgID("ContestSuperSimulator.ActiveSimulator");
            object o = Activator.CreateInstance(t);
            var v1 = o.GetType();
            css = v1.InvokeMember("Current", BindingFlags.GetProperty, null, o, null);
            cssType = css.GetType();
        }

        public Radio GetSimulatorForRadio(short rIdx)
        {
            return new Radio(cssType.InvokeMember("GetSimulatorForRadio", BindingFlags.InvokeMethod, null, css, new object[1] { rIdx }));
        }

        public CwSideToneSimulator GetCwSidetoneGenerator(object notify) // copy/paste CwSidetoneNotifyExample below for the implementation
        {
            return new CwSideToneSimulator(cssType.InvokeMember("GetCwSidetoneGenerator", BindingFlags.InvokeMethod, null, css, new object[1] { notify }));
        }
    }

    // Any public class with the appropriate ComVisible attributes, DispId's and method signatures is OK for GetCwSidetoneGenerator
    [ComVisible(true)]
    public class CwSidetoneNotifyExample
    {
        [ComVisible(true), DispId(1)]
        public void CwQueued(int MessageId)
        {
        }
        [ComVisible(true), DispId(2)]
        public void CwFinished(int MessageId, string TextSent)
        {
        }
    }
}
