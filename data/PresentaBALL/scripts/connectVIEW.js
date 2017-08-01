/* 
 * Include this file (connectVIEW.js) in every webpage that shall be able
 * to react to VIEW messages. Please note that this file requires the Qt
 * WebChannel JS interface (qrc:///qtwebchannel/qwebchannel.js) to be
 * loaded as well.
 */
current_action = -1;

var MsgClass = {
		    "UNDEFINED" : -1,
		    "COMPOSITE" : 0,  
		    "REPRESENTATION": 1, 
		    "SCENE": 2,
		    "DATASET": 3
		};

var MsgTypeComp = { 
		// Undefinded (default) type
		"UNDEFINED" : -1,
		
		
		//Composite ENUMS
		/* A new composite to be added 
		 		MolecularStructure will add bonds, normalize names and send a msg with type NEW_MOLECULE afterwards.
		*/
		"NEW_COMPOSITE": 0,
		
		// A Composite to be removed
		"REMOVED_COMPOSITE": 1,
		
		// Update all datas for a Composite (but not in the MolecularControl)
		"CHANGED_COMPOSITE": 2,
		
		// Update all datas for a composite (also in MolecularControl)
		"CHANGED_COMPOSITE_HIERARCHY": 3,
		
		// selected a composite (e.g. per checkboxes in MolecularControl)
		"SELECTED_COMPOSITE": 4,
		
		// deselected a composite (e.g. per checkboxes in MolecularControl)
		"DESELECTED_COMPOSITE": 5,
		
		// center the camera on a composite
		"CENTER_CAMERA": 6
		/* MolecularControl will add the Composite.
		  	DisplayProperties will create a Representation
		*/
		};
var MsgTypeSce = {		
		// Undefinded (default) type
		"UNDEFINED" : -1,
		
		//Build new molecule
		"NEW_MOLECULE": 0,

		// Rebuild the GLDisplayList objects in the GLRenderer
		"REBUILD_DISPLAY_LISTS": 1,
		
		// Redraw from the GLDisplayList objects
		"REDRAW": 2,

		// Move the Camera in the Scene to the value in this message
		"UPDATE_CAMERA": 3,
		
		// Remove the coordinate system in the Scene.
		"REMOVE_COORDINATE_SYSTEM": 4,

		// Export a PNG
		"EXPORT_PNG": 5,

		// Export a POVRay
		"EXPORT_POVRAY": 6,

		// 
		"ENTER_ROTATE_MODE": 7,

		// 
		"ENTER_PICKING_MODE": 8,

		// Move Composites or a Clipping Plane
		"ENTER_MOVE_MODE": 9,

		// A previous export has finished
		"EXPORT_FINISHED": 10
		};
		
var MsgTypeRep = {	
		// Undefinded (default) type
		"UNDEFINED" : -1,
		
		// Add a Representation
		"ADD": 0,

		// Remove a Representation
		"REMOVE": 1,

		// Update the Representation
		"UPDATE": 2,

		// Representation was selected in GeometriControl
		"SELECTED": 3,

		// Started the update of a Representation, no changes allowed in GeometricControl
		"STARTED_UPDATE": 4,

		// Finished the update of a Representation
		"FINISHED_UPDATE": 5,

		// Add a Representation to GeometricControl, but dont update in Scene
		"ADD_TO_GEOMETRIC_CONTROL": 6
	}; 

var MsgTypeDataSet = {
			/// Default Value
			"UNDEFINED": -1,

			/// To add 
			"ADD": 0,

			/// To be removed 
			"REMOVE": 1,

			/// Update 
			"UPDATE": 2,

			/// Was selected in Control
			"SELECTED": 3,

			///
			"VISUALIZE": 4,

			/// Allow >90 different kinds of visualization
			"VISUALIZE_END": 100
		};

function onJSMessage(i, j) { //i = Message Class, j = Message Type
	switch (current_step)
	{
	  case 1: // Create a peptide
	  {
		  if (j != MsgTypeSce.NEW_MOLECULE) return; 
	    
		  document.getElementById('button1').disabled=false;
		  
		  
		  break;
	  }
	  case 2: // Focusing
	  {
		  if (i != MsgClass.SCENE) return;
		  document.getElementById('button1').disabled=false;
		  break;
	  }
	  case 3: // Hierarchy
	  {
		    document.getElementById('button1').disabled=false;
		    break;
	  }
	  case 4: //Molecular Dynamics Simulation
	  {
		    
		   if (i != MsgClass.DATASET) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  case 5: // Visualisation of trajectories
	  {
		    
		   if (j != MsgTypeComp.CHANGED_COMPOSITE) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  case 6: // Calculation of electrostatics
	  {
		    
		   if (i != MsgClass.DATASET && j != MsgTypeDataSet.ADD) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  case 7: // Creating a Solvent Excluded Surface
	  {
		    
		   if (j != MsgTypeRep.ADD_TO_GEOMETRIC_CONTROL) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  case 8: // Coloring a SES by electrostatics
	  {
		    
		   if (j != MsgTypeRep.UPDATE) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  case 9: // Creating a isocontour surface
	  {
		    
		   if (j != MsgTypeRep.ADD) return;
		   
		  document.getElementById('button1').disabled=false;  
		  break; 
	  }
	  
	  case 10: // Ending
	  { 
		  break; 
	  }
	
	}
	
}//end function onJSMessage


function onJSActionSignal(i) { //i = ActionType
	
	if (current_action == i)
	{
		return;
	}
	current_action = i;

	// alert("Action Type " + i);
	switch (current_step)
	{
	  case 11: // optimizing = 2
	  {
		  if (i != 94) return; 
	    
		  document.getElementById('button1').disabled=false;
		  
		  break;
	  }
	  case 12: // bondorder = 3
	  {
		  if (i != 2) return;
		 
		  document.getElementById('button1').disabled=false;
		 
		  break;
	  }
	  case 13: // add_hydrogens = 1
	  {
		  if (i != 95) return;
		  
		  document.getElementById('button1').disabled=false;
		  
		  break;
	  }
	  case 14: // Ending
	  { 
		  break; 
	  }
	//break;
	}
	
}

window.onload = function() {
	try
	{
		new QWebChannel(qt.webChannelTransport, function(channel) {
			channel.objects.signals.actionSignal.connect(onJSActionSignal);
			channel.objects.signals.messageSignal.connect(onJSMessage);
		});
	}
	catch(e)
	{
		alert(e);
	}
}
