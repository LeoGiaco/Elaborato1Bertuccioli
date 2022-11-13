Istruzioni per l'esecuzione:

🞄 Linux/uso diretto di g++:
│
└─── Scaricare la cartella, inserirci dentro una cartella di nome "bin" e lanciare da terminale il comando "make && ./main.out".

🞄 Visual Studio:
│
└─┬─ Creare un nuovo progetto vuoto in C++ e inserire la cartella scaricata nella cartella del progetto;
  ├─ Aggiungere tutti i file .h e .cpp al progetto dalla barra laterale "Esplora soluzioni", cliccando rispettivamente su 
  │  "File di intestazione" per i file .h e "File di origine" per i file .cpp (tasto destro -> Aggiungi -> Elemento esistente);
  ├─ Aprire il file "main.cpp" dentro la cartella e modificare la macro "PROJECT_FOLDER" con il nome della cartella scaricata seguito da "/";
  └─ Lanciare in esecuzione il progetto normalmente tramite Visual Studio.
