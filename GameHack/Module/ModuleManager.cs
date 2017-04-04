using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameHack.Module.Modules;
using UnityEngine;

namespace GameHack.Module
{
	public class ModuleManager
	{
		private List<Module> modules = new List<Module>();

		public static ModuleManager instance;

		public ModuleManager()
		{
			instance = this;
			registerModule(new UnlimitedAmmo("Infinite Ammo", KeyCode.P));
			registerModule(new GodMode("God Mode", KeyCode.G));
			registerModule(new Esp("Esp", KeyCode.H));
		}

		private void registerModule(Module module)
		{
			modules.Add(module);
		}

		public List<Module> getModules()
		{
			return modules;
		}
	}
}
