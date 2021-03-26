/*
    IE3032 - PROCESAMIENTO DE SEÑALES
    DENNY STEVE OZTOY CHEX, 18476
    LAB 5 - ALMACENAMIENTO SD

    Basado en el ejemplo listfiles.ino de la librería SD:
      creado   Nov 2010
      por David A. Mellis
      modificado 9 Apr 2012
      por Tom Igoe
      modificado 2 Feb 2014
      por Scott Fitzgerald  
    
      "This example code is in the public domain."
   
*/
//*****************************************************************************************************
//                                          LIBRERIAS
//*****************************************************************************************************

#include <SPI.h>
#include <SD.h>

//*****************************************************************************************************
//                              VARIABLES, CONSTANTES Y DEFINICIONES
//*****************************************************************************************************

File root; // Objeto File usado por la librería SD

int lect = 0; // Variable que recibe los datos de la consola
bool NewLect = false, validLect = false, noSD = false; // Banderas para control de datos
String name = "", content = ""; // Contienen los nombres de los archivos y el contenido de la SD

//*****************************************************************************************************
//                                            SETUP
//*****************************************************************************************************

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.setModule(0); // Se elige el puerto SPI numero 0 de la Tiva
  
  pinMode(PA_3, OUTPUT); // Pin para Chip Select del maestro

  if (!SD.begin(PA_3)) { // Comprueba que la tarjeta SD esté conectada
    Serial.println("initialization failed!");
    noSD = true; // Bandera que indica si hay una tarjeta SD
    return; // Si no la hay, deja de configurar
  }
  
  root = SD.open("/"); // Mueve el "cursor" a la posicion indicada ("/")
  printDirectory(root, 0); // Lee los directorios y los guarda en una variable.
}

//*****************************************************************************************************
//                                            LOOP
//*****************************************************************************************************

void loop()
{
  if (!NewLect && !noSD) { // Verifica que haya una nueva lectura y que haya una tarjeta SD
    menu(); // Muestra el menu de opciones elegibles por el usuario
    NewLect = true; // Deshabilita el menú hasta que sea posible hacer otra lectura de la consola
  }
  if (Serial.available() > 0) { // Si hay datos provenientes de la terminal, se leen
    if (Serial.available() == 1) {
      NewLect = false; // Se reinicia la bandera de lecturas
    }
    lect = Serial.read();

    // Si la opcion no está en el menú ejecuta este error. (49 y 52 son valores asci para 1 y 4)
    // (10 en ascii es \n)
    if ((lect > 52 || lect < 49) && lect != 10) { 
      Serial.println("OPCION DESCONOCIDA.");
      delay(1000); // espera 1 segundo antes de "borrar" la pantalla
      for (uint8_t i = 0; i < 15; i++) {
        Serial.println(" "); // imprime muchos espacios para "borrar" lo que se mostró antes
      }
    } else if (lect != 10) { // si la lectura no es 10 (nueva linea en ASCII) valida la entrada
      validLect = true; // activa la bandera de lectura valida
      switch (lect) { // Segun el dato recibido ejecuta una opcion del menu
        case 49: // Mostrar imagen 1
          name = "sword.txt";
          break;
        case 50: // Mostrar imagen 2
          name = "cat.txt";
          break;
        case 51: // Mostrar imagen 3
          name = "banana.txt";
          break;
        case 52: // Mostrar contenido de la SD
          name = "/";
          validLect = false; // invalida la lectura, para no ejecutar el siguiente bloque
          //root = SD.open("/");
          //printDirectory(root, 0);
          Serial.println(content); // Imprime el contenido encontrado en la SD
          break;
      }
    }
    Serial.println(""); // Espacio usado como separador
  }

  if (validLect) { // Bloque que valida la lectura de la consola
    validLect = false; // Desactiva la bandera de lectura valida
    if (name == "sword.txt") { // Segun la opcion recibida, 
      root = SD.open("/sword.txt"); // Se abre un documento u otro
    } else if (name == "cat.txt") {
      root = SD.open("/cat.txt");
    } else if (name == "banana.txt") {
      root = SD.open("/banana.txt");
    }

    if (root) { // Si el documento no está disponible
      while (root.available()) {
        Serial.write(root.read()); // Se lee
      }
      root.close(); // Y se cierra
    }
    else { // Si no está disponible
      Serial.println("error opening file"); // se notifica
    }
    Serial.println(""); //espacio de estética
  }
} // main();

//*****************************************************************************************************
//                                          FUNCIONES
//*****************************************************************************************************

// Menu principal
void menu(void) {
  Serial.println("1. Imprimir Espada");
  Serial.println("2. Imprimir Gatos");
  Serial.println("3. Imprimir Banana");
  Serial.println("4. Listar contenido de SD");
  Serial.println("  Seleccione la opcion numerica en la consola (1 numero a la vez)");
}

// Guarda el directorio de archivos en una variable string
void printDirectory(File dir, int numTabs) {// Recibe la dirección y una cantidad de tabulaciones
  while (true) {
    File entry =  dir.openNextFile(); // Lee los archivos
    if (! entry) { // Y si ya no hay
      break; // Sale del ciclo
    }
    for (uint8_t i = 0; i < numTabs; i++) { // tabulaciones introducidas
      content += '\t';
    }
    content += entry.name(); // guarda el nombre del archivo
    if (entry.isDirectory()) { // Si es un directorio
      content += "/ \n"; // Genera una nueva linea
      printDirectory(entry, numTabs + 1); // Y comienza de nuevo desde este directorio + 1 tabulacion
    } else { // Si es un archivo
      content += "\t\t"; // Deja espacio
      content += entry.size(); // e introduce su tamaño en bytes
      content += "\n"; // se genera una nueva linea
    }
    entry.close(); // Cierra el archivo actual.
  }
}
