import os
import pdb
from pydrive.files import GoogleDriveFile
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive
import pydrive


class Drive:

    MIMETYPES = {
        "folder": "application/vnd.google-apps.folder",
        "document": "application/vnd.google-apps.document",
    }

    def __init__(self):
        self.gauth = GoogleAuth()
        self.drive = GoogleDrive(self.gauth)

    def get_metadata(self, file: GoogleDriveFile):
        file.FetchMetadata()

    def get_GoogleDriveFile(self, id_: str) -> GoogleDriveFile:
        return self.drive.CreateFile({"id": id_})

    def get_parent(self, file: GoogleDriveFile) -> str:
        return file["parents"][0]["id"]

    def get_mimetype(self, file: GoogleDriveFile) -> str:
        try:
            return file["mimeType"]
        except pydrive.files.ApiRequestError:
            raise RuntimeWarning("File %s does not exist" % file)

    def get_path_id(self, path: str, parent: str = "root") -> str:
        paths = os.path.normpath(path).split(os.sep)
        files = self.list_folder(parent)

        found_ids = {parent}

        # pdb.set_trace()

        for p in paths:
            folder_paths = found_ids
            found_ids = set()
            for id_ in folder_paths:
                files = self.list_folder(id_, mimetype=None)
                for f in files:
                    if f["title"] == p:
                        last_id = f["id"]
                        found_ids.add(f["id"])
            if not found_ids:
                raise RuntimeWarning(
                    "Could not find file/directory {} from path {}".format(p, path)
                )

        return found_ids

    def get_id_path(self, id_: str) -> str:
        pass

    def list_folder(self, parent: str, mimetype: str = None):
        files = []
        file_list = self.drive.ListFile(
            {"q": "'%s' in parents and trashed=false" % (parent,)}
        ).GetList()
        for file in file_list:
            try:
                if not mimetype or self.get_mimetype(file) == mimetype:
                    files.append(file)
            except RuntimeWarning:
                raise RuntimeError(
                    "Could not get the mimetype of a file stored in drive: %s" % file
                )

        return files

    def create_folder(self, name: str, parent_id: str = "root"):
        folder = self.drive.CreateFile(
            {
                "title": name,
                "mimeType": self.MIMETYPES["folder"],
                "parents": [{"id": parent_id}],
            }
        )
        folder.Upload()
        ...

    def delete_folder(self, id_: str, path: str = None) -> int:
        if not id_:
            if path:
                id_ = self.get_path_id(path)
            else:
                raise RuntimeWarning("Either a folder ID or path must be provided.")
        elif isinstance(id_, str):
            id_ = {id_}

        for i in id_:
            folder = self.get_GoogleDriveFile(i)
            try:
                if self.get_mimetype(folder) != self.MIMETYPES["folder"]:
                    raise RuntimeWarning(
                        "Cannot delete folder as ID='%s' is not a folder" % i
                    )
                folder.Trash()
                folder.Upload()
            except RuntimeWarning:
                return 1
        return 0

    def save_img(self, name: str, path: str):
        img_file = self.drive.CreateFile({"title": name})
        img_file.SetContentFile()
        ...


if __name__ == "__main__":
    # gauth = GoogleAuth()
    # gauth.LocalWebserverAuth()

    d = Drive()
    # pdb.set_trace()
    l = d.list_folder("root")

    # d.create_folder("testdir")
    print(d.get_path_id("testdir/cbt_isabelle.jpg"))

    d.delete_folder("1oViKmgY4Jv7wileFIGnSoiApQP7jkYCg")
    # l = d.list_folder('1MmqjLoYPDHAZf0T0T-L8dgG-y0LFkbv3')
    # for l_ in l:
    #     print("Folder: {}".format(l_), end='\n\n')
    # id_ = d.get_path_id("Folder 1/Folder 1.1/Hello.jpg")
    # print("ID: {}".format(id_))
    # f = d.get_GoogleDriveFile(id_)
    # print(f)
    # print(f.FetchMetadata())
    # print(f)
