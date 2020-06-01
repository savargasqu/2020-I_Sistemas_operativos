# Higher level (user facing) functions

- insert (ingresar):
  - calls request data from user
  - calls lookup
  - Probes to find a place to write the record (calling read)
  - calls write

- view (ver):
  - calls print number of records
  - calls requestID
  - Checks record exists
    - calls lookup
    - calls read
  | prints record info to stdin
  | opens record text file

- delete (borrar):
  - calls print number of records
  - calls requestID
  - calls write (writes a null record)

- search (buscar):
  - requests name
  - calls polyhash
  - Loops
    - calls lookup
    - calls read
    | if name coincides (case insensitive): prints ID
    | if name is null: break


# Auxiliary functions

- print number of records
- request ID

# Lower level (server facing) functions

- read: wrapper around fread

- write: wrapper around fwrite

- lookup: wrapper around fseek

- polyhash:
