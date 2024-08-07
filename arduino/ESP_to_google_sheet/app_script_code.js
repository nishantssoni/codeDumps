function doGet(e) { 
    Logger.log(JSON.stringify(e)); // Log the request parameters for debugging
    var result = 'Ok'; // Initialize result message
    
    if (Object.keys(e.parameter).length === 0) { // Check if there are no parameters in the request
      result = 'No Parameters'; // Set result message if no parameters are present
    } else {
      var sheet_id = '_________________________'; // Spreadsheet ID (Replace with your actual Spreadsheet ID)
      var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet(); // Open the spreadsheet by ID and get the active sheet
      var newRow = sheet.getLastRow() + 1; // Determine the next empty row
      var rowData = []; // Initialize an array to hold the row data
      var Curr_Date = new Date(); // Get the current date and time
      rowData[0] = Curr_Date; // Set the current date in column A
      var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Kolkata", 'HH:mm:ss'); // Format the current time for the specified time zone
      rowData[1] = Curr_Time; // Set the current time in column B
      
      // Loop through each parameter in the request
      for (var param in e.parameter) {
        Logger.log('In for loop, param=' + param); // Log the parameter name for debugging
        var value = stripQuotes(e.parameter[param]); // Remove quotes from the parameter value
        Logger.log(param + ':' + e.parameter[param]); // Log the parameter name and value for debugging
        
        switch (param) {
          case 'user_id':
            rowData[2] = value; // Set the User ID in column C
            result = 'User ID Written in column C'; // Update the result message
            break;
          case 'examiner_id':
            rowData[3] = value; // Set the Examiner ID in column D
            result += ' , Examiner ID Written in column D'; // Update the result message
            break; 
          default:
            // Check for parameters starting with 'e' or 'd'
            if (param.startsWith('e') || param.startsWith('d')) {
              var index = parseInt(param.substring(1)); // Extract the numeric part of the parameter
              if (!isNaN(index) && index > 0 && index <= 10) { // Ensure the index is a valid number within the expected range
                if (param.startsWith('e')) {
                  rowData[4 + (index - 1) * 2] = value; // Set 'e' parameters in columns E, G, I, ...
                  result += ` , ${param} Written in column ${String.fromCharCode(69 + (index - 1) * 2)}`; // Update the result message
                } else {
                  rowData[5 + (index - 1) * 2] = value; // Set 'd' parameters in columns F, H, J, ...
                  result += ` , ${param} Written in column ${String.fromCharCode(70 + (index - 1) * 2)}`; // Update the result message
                }
              } else {
                result = "unsupported parameter"; // Update result message for unsupported parameter
              }
            } else {
              result = "unsupported parameter"; // Update result message for unsupported parameter
            }
        }
      }
      Logger.log(JSON.stringify(rowData)); // Log the row data array for debugging
      var newRange = sheet.getRange(newRow, 1, 1, rowData.length); // Get the range for the new row
      newRange.setValues([rowData]); // Set the values in the new row
    }
    
    return ContentService.createTextOutput(result); // Return the result message as a text output
  }
  
  // Function to remove quotes from the parameter value
  function stripQuotes(value) {
    return value.replace(/^["']|['"]$/g, ""); // Remove leading and trailing quotes
  }
  