using UnityEngine;

// Include the namespace required to use Unity UI
using UnityEngine.UI;

using System.Collections;
using System.Collections.Generic;

public class PlayerController : MonoBehaviour {

    public float fX = 1f;
    public float fY = 9.81f;
    public float fZ = 1f;
    public ForceMode mode = ForceMode.Impulse;

    // Create private references to the rigidbody component on the player, and the count of pick up objects picked up so far
    private Rigidbody rb;
	private int count;
	private SerialController serialController;
	// At the start of the game..
	void Start ()
	{
		serialController = GameObject.Find("SerialController").GetComponent<SerialController>();
		// Assign the Rigidbody component to our private rb variable
		rb = GetComponent<Rigidbody>();

	}

	string message = null;
	string[] data = null;
    char[] delimiter = new char[1] { ';' };
    float x, y, z;


    int baseCapacity = 10;
    List<float> baseXs = new List<float>();
    List<float> baseYs = new List<float>();
    List<float> baseZs = new List<float>();
    float baseX = 0;
    float baseY = 0;
    float baseZ = 0;
    bool isBaseSet = false;
    // Each physics step..
    void FixedUpdate ()
	{
		 message = serialController.ReadSerialMessage();
		if (message != null)
		{
			// Check if the message is plain data or a connect/disconnect event.
			if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_CONNECTED))
				Debug.Log("Connection established");
			else if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_DISCONNECTED))
				Debug.Log("Connection attempt failed or disconnection detected");
			else
			{
                //data
				data = message.Split(delimiter , System.StringSplitOptions.RemoveEmptyEntries);
                if (data.Length == 3)
                {
                    x = float.Parse(data[0]);
                    z = float.Parse(data[1]);
                    y = float.Parse(data[2]);//mma z
                //    x = 1;
                 //   z = 1;

                    if (!isBaseSet)
                    {
                        baseXs.Add(x);
                        baseYs.Add(y);
                        baseZs.Add(z);
                        if (baseXs.Count > baseCapacity)
                        {
                            foreach (var item in baseXs)
                            {
                                baseX += item;
                            }
                            baseX = baseX / baseCapacity;

                            foreach (var item in baseYs)
                            {
                                baseY += item;
                            }
                            baseY = baseY / baseCapacity;

                            foreach (var item in baseZs)
                            {
                                baseZ += item;
                            }
                            baseZ = baseZ / baseCapacity;
                            isBaseSet = true;
                        }
                    }
                    else
                    {
                       /* x = Mathf.Round(x * 100f) / 100f;
                        y = Mathf.Round(y * 100f) / 100f;
                        z = Mathf.Round(z * 100f) / 100f;*/
                        //rb.AddForce(new Vector3(x * fX, y * fY, z * fZ), mode);
                        rb.AddForce(new Vector3((x-baseX) * fX, (y-baseY) * fY, (z-baseZ) * fZ), mode);
                        
                        //rb.AddForce(new Vector3((x/baseX)* fX, (y/baseY) * fY, (z/baseZ) * fZ), mode);
                        //rb.AddTorque(new Vector3((x / baseX) * fX, (y / baseY) * fY, (z / baseZ) * fZ), mode);
                    }
                    //rb.AddTorque(new Vector3(x * fX, y * fY, z * fZ), ForceMode.Impulse);
                    //rb.AddRelativeForce(new Vector3(x * fX, y * fY, z * fZ), ForceMode.Impulse);
                   // rb.AddRelativeTorque(new Vector3(x * fX, y * fY, z * fZ), ForceMode.Impulse);

                }
			}
		}
	
	}

	// When this game object intersects a collider with 'is trigger' checked, 
	// store a reference to that collider in a variable named 'other'..
	void OnTriggerEnter(Collider other) 
	{
		// ..and if the game object we intersect has the tag 'Pick Up' assigned to it..
		
	}

	// Create a standalone function that can update the 'countText' UI and check if the required amount to win has been achieved
}