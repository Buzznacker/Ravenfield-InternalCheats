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
	public class PayLoadFun : MonoBehaviour
	{
		

		//Called whenever there is a gui render.
		public void OnGUI()
		{
			GUI.color = Color.red;
			GUI.Label(new Rect(10, 10, 1000.0F, 1000.0F), "Cool internal cheats by GhosePvP(Buzznacker) lul");

			StringBuilder sb = new StringBuilder();
				
			ModuleManager.instance.getModules()
				.ForEach(mod => 
				{
					if (mod.isEnabled())
					{
						sb.Append("\n*").Append(mod.getName());
						mod.onRender();
					}
				});

			GUI.color = Color.green;
			GUI.Label(new Rect(10, 24, 1000, 1000), "Enabled Modules:" + sb.ToString());
		}

		public void Update()
		{		
			ModuleManager.instance.getModules()
				.ForEach(mod =>
				{
					if (Input.GetKeyDown(mod.getToggleKey()))
						mod.toggle();
				});
		}				
	}
}
