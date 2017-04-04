using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using UnityEngine;
using GameHack.Module;

namespace GameHack
{
    public class Hack
    {
		//Used for debugging.
		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		static extern int MessageBox(IntPtr hWnd, String text, String caption, uint options);

		public static GameObject payloadLoader;

		public static void Entry()
		{
			MessageBox(IntPtr.Zero, "Successfully injected", "Success", 0);

			//Initiate our cheat manager.
			new ModuleManager(); 

			//Add our gameobject, essentially our event handler for the game.
			payloadLoader = new GameObject();
			payloadLoader.AddComponent<PayLoadFun>();
			UnityEngine.Object.DontDestroyOnLoad(payloadLoader);			
		}	
    }
}
