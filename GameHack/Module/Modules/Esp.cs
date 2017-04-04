using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace GameHack.Module.Modules
{
	public class Esp : Module
	{
		public Esp(string name, KeyCode toggleKey) : base(name, toggleKey) {}

		public override void onRender()
		{
			ActorManager.AliveActorsOnTeam(1).ForEach(actor =>
			{
				DrawName(actor.transform.position, "Enemy #" + actor.GetInstanceID());
			});
		}

		private void DrawName(Vector3 pos, string name)
		{
			pos.y = pos.y + 2.5f;
			Vector2 screenPos = Camera.main.WorldToScreenPoint(pos);
			screenPos.y = (float)Screen.height - screenPos.y;
			GUI.color = Color.red;
			GUI.Label(new Rect(screenPos.x, screenPos.y, 100, 20), name);
		}
	}
}
