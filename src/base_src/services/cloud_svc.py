import os
from pydrive.files import GoogleDriveFile
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

class Drive():

    FOLDER = "application/vnd.google-apps.folder"

    def __init__(self):
        self.gauth = GoogleAuth()
        self.drive = GoogleDrive(self.gauth)

    def get_metadata(self, file:GoogleDriveFile):
        file.FetchMetadata()

    def get_GoogleDriveFile(self, id_:str) -> GoogleDriveFile:
        return self.drive.CreateFile({'id': id_})

    def get_parent(self, file:GoogleDriveFile) -> str:
        return file['parents'][0]['id']

    def get_path_id(self, path:str, root:str='root'):
        paths = os.path.normpath(path).split(os.sep)
        files = self.list_folder(root)

        last_id = root
        for p in paths:
            files = self.list_folder(last_id)
            for f in files:
                if f['title'] == p:
                    last_id = f['id']
                    break
            else:
                raise RuntimeWarning("Could not find file/directory {} from path {}".format(p, path))

        return last_id

    def list_folder(self, parent:str, mimetype:str=None):
        files = []
        file_list = self.drive.ListFile({'q': "'%s' in parents and trashed=false" % parent}).GetList()
        for file in file_list:
            if not mimetype or file['mimeType'] == mimetype:    
                files.append(file)
        return files


    def create_folder(self, name:str):
        ...

    def delete_folder(self, name:str):
        # Use Trash(), not Delete()
        ...

    def save_img(self, name:str, path:str):
        img_file = drive.CreateFile({'title':name})
        img_file.SetContentFile()
        ...

if __name__ == "__main__":
    gauth = GoogleAuth()
    gauth.LocalWebserverAuth()

    d = Drive()
    l = d.list_folder('1MmqjLoYPDHAZf0T0T-L8dgG-y0LFkbv3')
    for l_ in l:
        print(l_, end='\n\n')
    id_ = d.get_path_id("Folder 1/Folder 1.1/Hello.jpg")
    print(id_)
    f = d.get_GoogleDriveFile(id_)
    print(f)
    print(f.FetchMetadata())
    print(f)