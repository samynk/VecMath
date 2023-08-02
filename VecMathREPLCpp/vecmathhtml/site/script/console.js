var iCount = 0;
			var Module = {
				print: (function() {
					var element = document.getElementById('output');
					return function(text) {
						element.innerHTML += text + "<br>";
					};
				})(),
				printErr: function(text) {
						if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
						if (0) {
							dump(text + '\n');
						}
				},				
				canvas: (function() {
					var canvas = document.getElementById('canvas');
					return canvas;
				})()
			};
		
			function checkKey(e){
 				if (e.keyCode == 13 ){
					e.preventDefault();
					submitCommand();
 				}
			}
			
            function submitCommand()
			{
				var input = document.getElementById('input');
				// response
				var consoleDiv = document.getElementById('console');

				var outputGroup = document.createElement("div");
				outputGroup.className = 'inputGroup';
				input.id = 'oldInput'+(iCount++);
				input.readOnly = true;
				var commandDiv = document.createElement('div');

				console.log ( input.value );
				var result = Module.ccall('prompt','string',['string'],[input.value]);
				
				
				let copyButton = document.createElement('button');
				copyButton.innerText = 'Copy';
				copyButton.onclick = function() {
					navigator.clipboard.writeText(result).then(function() {
						/* clipboard successfully set */
					}, function() {
						/* clipboard write failed */
					});
				};
				outputGroup.appendChild(copyButton);
				
				let lines = result.split('\n');

        		// Create a text node and a <br> element for each line
        		for (let i = 0; i < lines.length; i++) {
            		let line = lines[i];
            		let textNode = document.createTextNode(line);
            		commandDiv.appendChild(textNode);

					// Add a <br> tag if it's not the last line
					if (i < lines.length - 1) {
						let br = document.createElement('br');
						commandDiv.appendChild(br);
					}
        		}
				
				//commandDiv.textContent = result;
				console.log(result);
				outputGroup.appendChild(commandDiv);
				
				consoleDiv.appendChild(outputGroup);
				
				// new input 
				var inputGroup = document.createElement('div');
				inputGroup.className = 'inputGroup';
				
				var prompt = document.createElement('div');
				prompt.textContent = 'vecmath>';
				prompt.className = 'prompt';
				inputGroup.appendChild(prompt);
				
				var newInput = document.createElement('input');
				newInput.type = 'text';
				newInput.id = 'input';
				newInput.className = 'cmdInput';
				newInput.autocomplete="off";
				newInput.setAttribute('autocorrect', 'off');
				newInput.spellcheck=false;
				newInput.onkeydown = checkKey;
				inputGroup.appendChild(newInput);
				consoleDiv.appendChild(inputGroup);
				newInput.focus();
			}