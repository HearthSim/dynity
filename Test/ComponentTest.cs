using System;
using UnityEngine;

namespace Test
{
    public class ComponentTest
    {
        public static void Run()
        {
            Debug.Log("Hello from Test.ComponentTest");

            var rootGO = ApplicationMgr.Get().gameObject;
            rootGO.AddComponent(typeof(FrameBehaviour));
        }
    }

    public class FrameBehaviour : MonoBehaviour
    {
        void Awake()
        {
            Log.Bob.Print("FrameBehaviour: Awake");
        }

        void Update()
        {
            Log.Bob.Print("FrameBehaviour: Update");
        }

        void FixedUpdate()
        {
            Log.Bob.Print("FrameBehaviour: FixedUpdate");
        }
    }
}
