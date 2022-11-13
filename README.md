Istruzioni per l'esecuzione:

<ul>
    <li>
        <h2>Linux/uso diretto di g++:</h2>
        <ul>
            <li>Scaricare la cartella, inserirci dentro una cartella di nome "bin" e lanciare da terminale il comando "make && ./main.out".</li>
        </ul>
    </li>
    <li>
        <h2>Visual Studio:</li>
        <ul>
            <li>Creare un nuovo progetto vuoto in C++ e inserire la cartella scaricata nella cartella interna del progetto (quella allo stesso livello del file .sln);</li>
            <li>Installare tramite la "Console di gestione pacchetti Nuget" i pacchetti "nupengl.core" e "glm" (comando "install-package");</li>
            <li>Aggiungere tutti i file .h e .cpp al progetto dalla barra laterale "Esplora soluzioni", cliccando rispettivamente su "File di intestazione" per i file .h e "File di origine" per i file .cpp (tasto destro -> Aggiungi -> Elemento esistente);</li>
            <li>Aprire il file "main.cpp" dentro la cartella e modificare la macro "PROJECT_FOLDER" con il nome della cartella scaricata seguito da "/";</li>
            <li>Lanciare in esecuzione il progetto normalmente tramite Visual Studio.</li>
        </ul>
    </li>
</ul>
