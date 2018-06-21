using UnityEngine;

// Include the namespace required to use Unity UI
using UnityEngine.UI;

using System.Collections;

public class PlayerController : MonoBehaviour {
	
	// Create public variables for player speed, and for the Text UI game objects
	public float speed;
	public Text countText;
	public Text winText;

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

		// Set the count to zero 
	//	count = 0;

		// Run the SetCountText function to update the UI (see below)
	//	SetCountText ();

		// Set the text property of our Win Text UI to an empty string, making the 'You Win' (game over message) blank
		//winText.text = "";
	}

	string message = null;
	string[] data = null;
    char[] delimiter = new char[1] { ';' };
    float x, y, z;
	float fX = 1;
	float fY = 1;
	float fZ = 1;
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
                    y = float.Parse(data[1]);
                    z = float.Parse(data[2]);
                    rb.AddForce(new Vector3(x * fX, y * fY, z * fZ), ForceMode.Acceleration);
                }


				//transform.Rotate(* Time.deltaTime);
			}
		}
		// Set some local float variables equal to the value of our Horizontal and Vertical Inputs
		float moveHorizontal = Input.GetAxis ("Horizontal");
		float moveVertical = Input.GetAxis ("Vertical");

		// Create a Vector3 variable, and assign X and Z to feature our horizontal and vertical float variables above
		Vector3 movement = new Vector3 (moveHorizontal, 0.0f, moveVertical);

		// Add a physical force to our Player rigidbody using our 'movement' Vector3 above, 
		// multiplying it by 'speed' - our public player speed that appears in the inspector
		rb.AddForce (movement * speed);
	}

	// When this game object intersects a collider with 'is trigger' checked, 
	// store a reference to that collider in a variable named 'other'..
	void OnTriggerEnter(Collider other) 
	{
		// ..and if the game object we intersect has the tag 'Pick Up' assigned to it..
		
	}

	// Create a standalone function that can update the 'countText' UI and check if the required amount to win has been achieved
}