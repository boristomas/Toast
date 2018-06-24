using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FanEvent : MonoBehaviour {

    Animation anim;
    ParticleSystem particle1;
    ParticleSystem particle2;
    Light spotLight;
    SerialController serialController = null;

    // Use this for initialization
    void Start () {
        anim = GetComponent<Animation>();
        particle1 = GameObject.Find("particleFanSpecs").GetComponent<ParticleSystem>();
        particle2 = GameObject.Find("particleFanFog").GetComponent<ParticleSystem>();
        spotLight = GameObject.Find("fanSpotlight").GetComponent<Light>();
        spotLight.enabled = false;

        serialController = GameObject.Find("SerialController").GetComponent<SerialController>();
    }
    private void OnMouseDown()
    {
        if (anim.isPlaying)
        {
            serialController.SendSerialMessage("foff");
            anim.Stop();
            particle1.Stop();
            particle2.Stop();
            spotLight.enabled = false;
        }
        else
        {
            serialController.SendSerialMessage("fon");
            anim.Play();
            particle1.Play();
            particle2.Play();
            spotLight.enabled = true;
        }
        
    }

    // Update is called once per frame
    void Update () {
		
	}
}
