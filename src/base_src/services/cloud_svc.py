from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

gauth = GoogleAuth()
gauth.LocalWebserverAuth()

drive = GoogleDrive(gauth)

class Drive():

    def __init__(self):
        self.gauth = GoogleAuth()
        self.gauth.LocalWebserverAuth()

        self.drive = GoogleDrive(self.gauth)

    def list_folder(parent:str):
        file_list = drive.ListFile({'q': "'%s' in parents and trashed=false"} % parent).GetList()
        for file1 in file_list:   
            if file1['mimeType'] == "application/vnd.google-apps.folder":    
                print ('title: %s, id: %s, type: %s' % (file1['title'], file1['id'], file1['mimeType']))

    def create_folder(name:str):
        ...

    def delete_folder(name:str):
        # Use Trash(), not Delete()
        ...

    def save_img(name:str, path:str):
        img_file = drive.CreateFile({'title':name})
        img_file.SetContentFile()
        ...