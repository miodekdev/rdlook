# RecursiveDirectoryLook
**Usage:** `./rdlook.exe <phrase> [options]`

Look for a phrase in filenames, directory names, and file contents within a directory or a file.

**Options:**
 - `/path, /p <path>`: Path to a directory or a file to be searched. If none provided, the path defaults to
the current directory.
 - `/recursive, /r`: Recursively looks through all subdirectories of the specified path.
 - `/silent, /s`: Only show the total amount of occurrences, and not their locations.
 - `/format, /f`: Format the output with ANSI escape codes.