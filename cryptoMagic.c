//todo fixed the encryption -- the value of the tab (9) isn't working or showing up 
#include <stdio.h>
#include <string.h>
int hextoDecimal(char hex);
void encryptFile(char *filename);
void decryptFile(char *filename);

int main(int argc, char **argv) {

  if (argc == 3) {
    if (strcmp(argv[1], "-E") == 0) {
      encryptFile(argv[2]);
    } else if (strcmp(argv[1], "-D") == 0) {
      decryptFile(argv[2]);
    }
  } else {
    encryptFile(argv[1]);
  }
}

//(hextoDecimal char) converts characters from a-f to hexidecimal values
int hextoDecimal(char hex) {
  if (hex == 'A') {
    return 10;
  } else if (hex == 'B') {
    return 11;
  } else if (hex == 'C') {
    return 12;
  } else if (hex == 'D') {
    return 13;
  } else if (hex == 'E') {
    return 14;
  } else if (hex == 'F') {
    return 15;
  } else {
    return hex - '0';
  }
}

void encryptFile(char *filename) {
  FILE *outputFile;
  FILE *file = fopen(filename, "r");
  char *basefile;
  char *finalfile;

  // Renames file *************************************************
  if (file == NULL) {
    printf("Input file does not exist");
    return;
  } else {
    basefile = strtok(filename, "."); // returns back with the first token
    finalfile =
        strcat(basefile, ".crp"); // take the token and add .crp at the end
  }

  char line[120]; // lines are guaranteed to have 120 characters or less

  outputFile = fopen(finalfile, "w+");

  while (fgets(line, 120, file) != NULL) { //if the line has anyhting on it then continue 
    for (int i = 0; i < strlen(line) - 1; i++) {
      int outChar = line[i];
      if (outChar==32){
        fprintf(outputFile, "%s",  "TT");
        continue;   
         

        // fwrite("TT", sizeof(char), 2, outputFile);
      }

      outChar = outChar - 16;

      if (outChar < 32) {
        outChar = (outChar - 32) + 144;
      }

      char result[3];
      sprintf(result, "%X", outChar);

      fwrite(result, sizeof(char), 2, outputFile);
    }

    fwrite("\n", sizeof(char), 1, outputFile); // going onto new line which may or may not have a string value
    outputFile =
        fopen(finalfile,"a+"); // unlike "w" that overwrites, "a" continues, and + r and w
  }

  fclose(file);
  fclose(outputFile);
}

void decryptFile(char *filename) {
  FILE *file = fopen(filename, "r");

  char *basefile = strtok(filename, ".");
  char *finalfile = strcat(basefile, ".txt");

  // create new .txt file
  FILE *outputFile = fopen(finalfile, "w+");

  char line[255];
  while (fgets(line, 255, file) != NULL) {
    for (int i = 0; i < strlen(line) - 1; i++) {
      int outChar = 0;

      if (line[i] == 'T' && line[i + 1] == 'T') {
        outChar = 9;
        i += 1;
      } else {
        outChar = hextoDecimal(line[i]) * 16;
        i += 1;
        outChar += hextoDecimal(line[i]);
        outChar += 16;

        if (outChar > 127) {
          outChar = (outChar - 144) + 32;
        }
      }

      fprintf(outputFile, "%c", outChar);
    }
    fwrite("\n", sizeof(char), 1, outputFile);
    outputFile = fopen(finalfile, "a+");
  }
  fclose(file);
}
