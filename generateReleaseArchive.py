import os
import zipfile

release_dir = r".\\out\\build\\Ninja-MSVC-Release"

release_version = input("enter release version (e.g. 5.3.1)\n")

output_zip = f".\\out\\build\\Ninja-MSVC-Release\\release-v{release_version}.zip"

folder_to_include = "assets"

def should_include(filename):
    return filename.endswith('.dll') or filename.endswith('.exe')

with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zipf:
    for root, dirs, files in os.walk(release_dir):
        if folder_to_include in os.path.relpath(root, release_dir).split(os.sep):
            include_all = True
        else:
            include_all = False

        for file in files:
            if include_all or should_include(file):
                file_path = os.path.join(root, file)
                arcname = os.path.relpath(file_path, release_dir)
                zipf.write(file_path, arcname)

print(f"Archive created: {output_zip}")