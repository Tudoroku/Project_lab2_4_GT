# Pentru compilare: pt Linux, ./build sau ./build-tests
# Rulare: ./run sau ./run-with-data pentru date preintroduse

## Rezumat rapid schimbari

- Proiectul folosește un singur modul de listă dinamică generică, implementat cu `void*`, în `Repository/list.*`.
- `Repo` este construit peste această listă generică și adaugă doar logica specifică dispozitivelor.
- `Undo` multiplu este implementat ca listă de liste: înainte de `add/update/delete` se salvează un snapshot complet și independent al repo-ului curent.
- La `undo`, starea curentă este înlocuită cu ultimul snapshot salvat, deci se poate reveni de mai multe ori.
- A fost adăugată o filtrare nouă după `tip`, pe lângă filtrările existente.
