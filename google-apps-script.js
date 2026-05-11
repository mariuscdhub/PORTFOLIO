function doPost(e) {
  try {
    // 1. Ouvrir le tableur actif (celui lié au script)
    var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
    
    // 2. Récupérer les données envoyées par le site web
    var data = JSON.parse(e.postData.contents);
    var newEmail = data.email.trim();
    
    if (!newEmail) {
      return response({ status: "error", message: "Email vide." });
    }
    
    // 3. Obtenir toutes les emails déjà enregistrées (en colonne A)
    var dataRange = sheet.getDataRange();
    var values = dataRange.getValues();
    var emailExists = false;
    
    // 4. Vérifier si l'email est déjà dans la liste
    for (var i = 0; i < values.length; i++) {
        // En supposant que l'adresse email est dans la première colonne (A)
        if (values[i][0] === newEmail) {
            emailExists = true;
            break;
        }
    }
    
    // 5. Si l'email n'existe pas, on l'ajoute
    if (!emailExists) {
        // Ajoute l'email et la date d'inscription
        sheet.appendRow([newEmail, new Date()]);
    }
    
    // 6. Retourner un succès
    return response({ status: "success", message: emailExists ? "Email déjà existant, ignoré." : "Email ajouté." });

  } catch (error) {
    return response({ status: "error", message: error.toString() });
  }
}

// Fonction utilitaire pour retourner la réponse
function response(data) {
  return ContentService.createTextOutput(JSON.stringify(data))
    .setMimeType(ContentService.MimeType.JSON);
}

// Permet de gérer les requêtes préliminaires (OPTIONS) envoyées par les navigateurs (CORS)
function doOptions(e) {
  return ContentService.createTextOutput("")
    .setMimeType(ContentService.MimeType.TEXT);
}
