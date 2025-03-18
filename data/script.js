function wifiCredRequest() {
	let ssidInput = document.getElementById("wifi-ssid");
	let passInput = document.getElementById("wifi-pass");

	let wifi = {
		ssid: ssidInput.options[ssidInput.selectedIndex].text.split(" ")[0].trim(),
		pass: passInput.value
	}

	let wifiJSON = JSON.stringify(wifi);

	console.log(wifiJSON);

	let xhr = new XMLHttpRequest();
  	xhr.open("POST", "/wifiCred", true);
  	xhr.setRequestHeader("Content-type", "text/plain");
  	xhr.send(wifiJSON);

  	xhr.onload  = function() {
  		if(xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
        	console.log(xhr.response);
    	}	
    };
}

function updateNetworks() {
	let button = document.getElementById("search-networks-button");
	button.innerHTML = "Loading...";
	button.disabled = true;
	let xhr = new XMLHttpRequest();
  	xhr.open("POST", "/searchNetworks", true);
  	xhr.setRequestHeader("Content-type", "text/plain");
  	xhr.send();

  	xhr.onload  = function() {
  		if(xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
        	let networksJson = JSON.parse(xhr.response);
        	let selectElement = document.getElementById("wifi-ssid");

        	if (networksJson["error"] != null) {
        		let i, L = selectElement.options.length - 1;
   				for(i = L; i >= 0; i--) {
      				selectElement.remove(i);
   				}

   				selectElement.options[0] = networksJson["error"];
        	} else {
        		let networksMap = new Map();
        		let networksArray = networksJson["networks"];
        		for (let el in networksArray) {
        			let wifi = {
        				ssid : networksArray[el]["ssid"],
        				rssi : networksArray[el]["rssi"],
        				hasPass : networksArray[el]["password"]
        			}

        			if (!networksMap.has(wifi.ssid)) {
        				networksMap.set(wifi.ssid, wifi);
        			} else {
        				let curwifi = networksMap.get(wifi.ssid);
        				if (parseInt(curwifi.rssi) < parseInt(wifi.rssi)) {
        					networksMap.set(wifi.ssid, wifi);
        				}
        			}
        		}

        		console.log(networksMap);

        		let option = 0;
        		let maxNameLength = 0;
        		for (let network of networksMap.keys()) {
        			let net = networksMap.get(network);
        			if (net.ssid.length > maxNameLength) 
        				maxNameLength = net.ssid.length;
        		}

        		for (let network of networksMap.keys()) {
        			let net = networksMap.get(network);
        			let level = "";
        			if (net.rssi > -35) level = "Perfect";
        			else if (net.rssi > -50) level = "Excellent";
        			else if (net.rssi > -65) level = "Good";
        			else if (net.rssi > -75) level = "Normal";
        			else if (net.rssi > -85) level = "Bad";
        			else level = "Horrible";

        			let optionText = net.ssid;
        			if (optionText.length <= maxNameLength) {
        				for (let i = optionText.length; i <= maxNameLength + 4; i++) {
        					optionText += "&nbsp;";
        				}
        			} 

        			selectElement.options[option] = new Option("", option);
        			selectElement.options[option].innerHTML = optionText + (net.hasPass == "true" ? " 🔐" : " 🔓") + "&nbsp;&nbsp;📶 " + level;
  					option++;
				}
        	}
    	}

    	button.innerHTML = "Search Networks";
		button.disabled = false;	
    };
}