# Cloud Setup

For the cloud setup, we will use the same Google account that was used for the email service because the cloud files will be stored on Google Drive.

In order to access Google Drive, you need to set up an authentication service.
This can be done in [Google Cloud](console.cloud.google.com).

1. Go to the [Google Cloud Platform console](console.cloud.google.com).
1. Click on "APIs & Services" in the sidebar, then click on "Create Project" (if you already have a project set up, click on the name of the project at the top and press "New Project").
1. Give your project a name, keep the location set to "No organisation", and press "Next".
1. With your new project set up, click "Enable APIs and Services".
1. Search for "Google Drive API" and press "Enable".
1. Go to the "Credentials" tab in the sidebar and click on "Configure Consent Screen". If you can't see this, then it is probably already set up.
    1. On the "OAuth consent screen page", set the user type to External.
    1. Set the app name to something meaningful and set the user support email. There is no need to add an app logo. Put your personal email address in the "Developer contact information" field.
    1. Press "Save and Continue" on the remaining pages and then finally press "Return to dashboard".
1. Return to the "Credentials" tab, click "Create Credentials", and choose "OAuth client ID". If one already exists, you can skip this step.
    1. Set the Application type to "Web application" and set a name. 
    1. Set the Authorised Javascript origins to `http://localhost:8090`. (The official Google documentation recommends using `8080`, so try that if `8090` doesn't work.)
    1. Set the Authorised redirect URIs to `http://localhost:8090/`. Don't forget the slash at the end of this one.
    1. Click "Create" and then "OK" on the window that pops up.
1. On the "Credentials" page, click on the download button for your new credential in the OAuth 2.0 Client IDs table.
1. Rename the file from `client_secret_<id_number>.json` to `client_secrets.json`. Don't forget the `s` at the end of `secrets`. Save this file to the `services/` directory so it can be accessed by `cloud_svc.py`.
1. In the `services/` directory, run `cp settings_example.yaml settings.yaml` to make a copy of the settings file.
1. Copy the client ID and client secret from `client_secrets.json` to the appropriate fields in `settings.yaml`.
1. On the Raspberry Pi, run `python cloud_svc.py`.
    1. Your default web browser will open and ask you to accept the connection to the app. If you see a page saying "This app isn't verified", just click "Show Advanced" and "Go to `app_name`". This is safe to do because *you* made the app.
    1. Allow everything that the app asks for. When done, you will see a message in the terminal saying "Authentication successful". This will create a credentials file which allows the device to authenticate itself headless.
