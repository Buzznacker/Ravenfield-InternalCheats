using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace GameHack.Module
{
	public abstract class Module
	{
		private String name;
		private KeyCode toggleKey;
		private bool enabled;

		protected ActorManager am = ActorManager.instance;

		public Module(String name, KeyCode toggleKey)
		{
			this.name = name;
			this.toggleKey = toggleKey;
		}

		public void toggle()
		{
			if(!enabled)
			{
				enabled = true;
				onEnable();
			}
			else
			{
				enabled = false;
				onDisable();
			}
		}

		public virtual void onEnable() {}

		public virtual void onDisable() {}

		public bool isEnabled() => enabled;

		public String getName() => name;

		public KeyCode getToggleKey() => toggleKey;
		
		public abstract void onRender();
	}
}
