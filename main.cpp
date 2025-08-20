#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>

using namespace std;

#define Nchars 69  // Total number of encipherable characters
#define Mchars 70  // Buffer size for strings containing Nchars
#define Nrotors 11 // Maximum number of rotors (1-based: 1-10)
#define Nrefls 5   // Total number of reflectors (1-based: 1-4)
#define Nsteps 11  // Maximum total number of encryption steps

// Rotor wirings
char *ROTOR[Nrotors] = {
    "abcdefghijklmnopqrstuvwxyz0123456789.,:; ()[]'\"-+/*&~`!@#$%^_={}|\\<>?", // Rotor 0
    "ekmflgdqvzntowyhxuspaibrcj4.:5,63)-&;' +*7/\"](081[29?><\\|}{=^_%$#@!`~", // Rotor 1
    "ajdksiruxblhwtmcqgznpyfvoe093.]8[\"/1,7+':2)6&;(*5- 4?><\\|}{=^_%$#@!`~", // Rotor 2
    "bdfhjlcprtxvznyeiwgakmusqo13579,2(['/-&;*48+60.:\"]) ?><\\|}{=^_%$#@!`~", // Rotor 3
    "esovpzjayquirhxlnftgkdcmwb4] -(&90*)\"8[7/,;5'6.32:+1?><\\|}{=^_%$#@!`~", // Rotor 4
    "vzbrgityupsdnhlxawmjqofeck-&1[68'*\"(]3;7,/0+:9) 542.?><\\|}{=^_%$#@!`~", // Rotor 5
    "jpgvoumfyqbenhzrdkasxlictw9(6- \":5*)14;7&[3.0]/,82'+?><\\|}{=^_%$#@!`~", // Rotor 6
    "nzjhgrcxmyswboufaivlpekqdt;&976[2/:*]+1 \"508-,(4.)3'?><\\|}{=^_%$#@!`~", // Rotor 7
    "fkqhtlxocbjspdzramewniuygv5.)7',/ 219](3&[0:4+;8\"*6-?><\\|}{=^_%$#@!`~", // Rotor 8
    "leyjvcnixwpbqmdrtakzgfuhos,4*9-2;8/+(1):3['0.&65\"7 ]?><\\|}{=^_%$#@!`~", // Beta rotor
    "fsokanuerhmbtiycwlqpzxvgjd5] .0;\"4[7:1'8*2+,)(&/-693?><\\|}{=^_%$#@!`~"  // Gamma rotor
};

// Notch positions
char NOTCH[Nrotors] = { 'z', 'q', 'e', 'v', 'j', 'z', 'z', 'z', 'z', 'a', 'a' };

// Reflectors
char *REFLECTOR[Nrefls] = {
    "abcdefghijklmnopqrstuvwxyz0123456789.,:; ()[]'\"-+/*&~`!@#$%^_={}|\\<>?", // Reflector 0
    "yruhqsldpxngokmiebfzcwvjat*[\"7)],3(/;6 .:8415&2+-90'?<>\\|}{=_^%$#@`!~", // Reflector B (Thick)
    "fvpjiaoyedrzxwgctkuqsbnmhl5-(980 *43[&/+62'.\")]1;:7,?<>\\|}{=_^%$#@`!~", // Reflector C (Thick)
    "enkqeuywjicopblmdxzvfthrgs4;.)0\"*+982 (1,:3/&-5'7[6]?<>\\|}{=_^%$#@`!~", // Reflector B (Dunn)
    "rdobjntkvehmlfcwzrxgyipsuq[3 19;'.-47:,52+&0/6*8(]\")?<>\\|}{=_^%$#@`!~"  // Reflector C (Dunn)
};

// Plugboard default wiring
char *PLUGBOARD = "abcdefghijklmnopqrstuvwxyz0123456789.,:; ()[]'\"-+/*&~`!@#$%^_={}|\\<>?";
char *alphabet = "abcdefghijklmnopqrstuvwxyz0123456789.,:; ()[]'\"-+/*&~`!@#$%^_={}|\\<>?";

// Machine settings and buffers
int mRotors, mSteps;
int RotPos[Nrotors];
char window[Nrotors], Iwindow[Nrotors];
char *RotWiring[Nrotors];
char RotNotch[Nrotors];
int RotNumber[Nrotors];
char *reflector, plugboard[Mchars];
int ReflType;
char step[Nsteps];

// File pointers and buffers
FILE *inFp, *outFp, *logFp;
char inLine[255], outLine[255];

void InitEnigma() // Default initialization
{
    int i;

    mRotors = 3; // Default number of rotors
    mSteps = (mRotors << 1) + 3; // Total encryption steps
    strcpy(plugboard, PLUGBOARD); // Initialize plugboard with default wiring

    for (i = 0; i <= mRotors; ++i) {
        RotWiring[i] = ROTOR[i];    // Set rotor wiring
        RotNotch[i] = NOTCH[i];     // Set rotor notches
        RotNumber[i] = i;           // Assign rotor numbers
        Iwindow[i] = window[i] = 'a'; // Set initial positions to 'a'
    }

    reflector = REFLECTOR[1]; // Default reflector (B)
    ReflType = 1;
}

int index(char c)
{
    int i = 0;

    while ((i < Nchars) && (c != alphabet[i])) {
        ++i;
    }
    return i; // Returns the index of the character in the alphabet array
}

void SetPlugboard() // Configure plugboard connections
{
    int i, n, x;
    char p1, p2, ch;

    fgets(inLine, 255, inFp); // Read the plugboard settings
    inLine[strlen(inLine) - 1] = '\0'; // Remove trailing newline
    n = strlen(inLine);

    for (i = 0; i < n; i += 2) {
        p1 = inLine[i];   // First character in the pair
        p2 = inLine[i+1]; // Second character in the pair
        x = index(p1);

        if ((ch = plugboard[x]) != p1) { // If occupied, disconnect
            plugboard[index(ch)] = ch;
            plugboard[x] = p1;
        }
        plugboard[x] = p2; // Plug in
        x = index(p2);

        if ((ch = plugboard[x]) != p2) { // If occupied, disconnect
            plugboard[index(ch)] = ch;
            plugboard[x] = p1;
        }
        plugboard[x] = p1; // Plug in
    }
}

int ChrToInt(char c)
{
    return (int)(c - '0'); // Converts a digit character to an integer
}

void PlaceRotor( int position, int r )
// set wirings of a single rotor
{
   RotWiring[ position ] = ROTOR[ r ];
   RotNotch[ position ] = NOTCH[ r ];
   RotNumber[ position ] = r;
}

void SetRotorsAndReflector()
{
    int i, n, rotor, rotPos;
    char ch, ringPos;

    fgets(inLine, 255, inFp); // Read the number of rotors
    mRotors = ChrToInt(inLine[0]);
    if (mRotors > 4) mRotors = 4; // Limit to 4 rotors
    mSteps = (mRotors << 1) + 3;

    for (i = 1; i <= mRotors; ++i) {
        fgets(inLine, 255, inFp); // Read rotor configuration
        ch = inLine[0];
        if (isdigit((int)ch))
            rotor = ChrToInt(ch); // Convert digit to rotor ID
        else {
            ch = tolower(ch);
            rotor = (ch == 'b') ? 9 : (ch == 'g') ? 10 : 0; // Beta/Gamma
        }
        rotPos = ChrToInt(inLine[1]); // Rotor position
        ringPos = inLine[2];          // Initial ring character
        Iwindow[rotPos] = window[rotPos] = ringPos;
        PlaceRotor(rotPos, rotor);    // Place rotor in position
    }

    fgets(inLine, 255, inFp); // Read reflector designation
    ch = inLine[0];
    switch (ch) {
        case 't': n = 0; break;
        case 'b': n = 1; break;
        case 'c': n = 2; break;
        case 'B': n = 3; break;
        case 'C': n = 4; break;
        default: n = 0; break;
    }
    reflector = REFLECTOR[n];
    ReflType = n;
}

void TryUserSetup() // Attempt initialization from user file
{
    if ((inFp = fopen("esetup", "rt")) != NULL) {
        SetPlugboard();           // Configure the plugboard
        SetRotorsAndReflector();  // Configure rotors and reflector
        fclose(inFp);             // Close the setup file
    }
}





int OpenFiles(char *inFname, char *encFname, char *logFname)
{
    inFp = fopen(inFname, "rt");   // Open input file
    outFp = fopen(encFname, "wt"); // Open output file
    logFp = fopen(logFname, "wt"); // Open log file
    return (inFp != NULL) && (outFp != NULL) && (logFp != NULL); // Success check
}

void CloseFiles()
{
    fclose(inFp);
    fclose(outFp);
    fclose(logFp);
}

void SetRotorPositions()
{
    for (int i = 1; i <= mRotors; ++i) {
        int j = RotNumber[i];
        char ch = window[j];
        char *Rwiring = RotWiring[j];
        int k = 0;
        while (Rwiring[k] != ch) {
            ++k;
        }
        RotPos[j] = k; // Set rotor position
    }
}

void ReportMachine()
{
    fprintf(logFp, "Plugboard mappings:\n");
    fprintf(logFp, "%s\n", ROTOR[0]); // Input alphabet
    fprintf(logFp, "%s\n", plugboard); // Current plugboard configuration

    fprintf(logFp, "\nRotor wirings:\n");
    fprintf(logFp, "Position Rotor Ring Setting Notch Sequence\n");
    for (int i = mRotors; i >= 1; --i) {
        fprintf(logFp, "%8d %5d %12c %5c %s\n",
                i, RotNumber[i], window[i],
                RotNotch[i], RotWiring[i]);
    }
    fprintf(logFp, "\nReflector %c %s\n", ReflType, reflector);
}

int mod(int n, int modulus)
{
    while (n >= modulus) n -= modulus;
    while (n < 0) n += modulus;
    return n;
}

void TurnRot(int n, int width)
{
    if (width > 0) {
        RotPos[n] = mod(RotPos[n] + width, Nchars); // Update rotor position
        window[n] = RotWiring[n][RotPos[n]];       // Update window display
    }
}

void turn()
{
    int doit[Nrotors] = {0}; // Flags to determine which rotors must turn
    doit[1] = 1; // The rightmost rotor always turns

    // Double-stepping mechanism
    for (int i = 2; i <= mRotors; ++i) {
        if (RotNotch[i - 1] == RotWiring[i - 1][RotPos[i - 1]]) {
            doit[i] = 1;
        }
    }

    // Apply turning to all rotors
    for (int i = 1; i <= mRotors; ++i) {
        TurnRot(i, doit[i]);
    }
}

char RtoLpath(char c, int r)
{
    int n, offset, idx, ret;
    char *CurRotor = RotWiring[r];

    n = index(c);
    offset = index(CurRotor[RotPos[r]]);
    idx = mod(n + offset, Nchars); // Apply rotor offset
    ret = mod(index(CurRotor[idx]) - offset, Nchars);

    return alphabet[ret];
}

char LtoRpath(char c, int r)
{
    int n, m, offset, idx, newchar;
    char *CurRotor = RotWiring[r];

    n = index(c);
    offset = index(CurRotor[RotPos[r]]);
    newchar = alphabet[mod(n + offset, Nchars)]; // Apply offset

    m = 0;
    while (m < Nchars && CurRotor[m] != newchar) {
        ++m;
    }
    idx = mod(m - offset, Nchars);

    return alphabet[idx];
}

char encrypt(char c)
{
    int i, r;

    turn(); // Move rotors as per the stepping mechanism

    i = 0; // Start encryption process
    step[i++] = plugboard[index(c)]; // Pass through plugboard

    // Right-to-left path through rotors
    for (r = 1; r <= mRotors; ++r) {
        step[i++] = RtoLpath(step[i - 1], r);
    }

    // Reflector
    step[i++] = reflector[index(step[i - 1])];

    // Left-to-right path through rotors
    for (r = mRotors; r >= 1; --r) {
        step[i++] = LtoRpath(step[i - 1], r);
    }

    // Back through plugboard
    step[i] = plugboard[index(step[i - 1])];

    return step[i]; // Final encrypted character
}

void ShowWindow()
{
    for (int i = mRotors; i >= 1; --i) {
        fprintf(logFp, "%c ", window[i]);
    }
    fprintf(logFp, "  ");
}

void ShowSteps()
{
    for (int i = 0; i < mSteps; ++i) {
        fprintf(logFp, " -> %c", step[i]);
    }
}

void ProcessPlainText()
{
    int i, n;
    char c1, c2;

    fprintf(logFp, "\n\nEncryption\n");
    while (fgets(inLine, 255, inFp) != NULL) {
        n = strlen(inLine) - 1; // Adjust length, ignoring newline
        inLine[n] = '\0';

        for (i = 0; i < n; ++i) {
            c1 = inLine[i];
            if (isupper((int)c1)) {
                c1 = tolower(c1); // Convert uppercase to lowercase
            }

            c2 = encrypt(c1); // Encrypt the character

            ShowWindow(); // Log rotor positions
            fprintf(logFp, " %c", c1);
            ShowSteps(); // Log encryption steps
            fprintf(logFp, "\n");

            outLine[i] = c2; // Store encrypted character
        }

        fprintf(logFp, "\n");
        outLine[i] = '\0';
        fprintf(outFp, "%s\n", outLine); // Write encrypted line to output file
    }
}

void ProcessFile(char *inFname, char *encFname, char *logFname)
{
    if (OpenFiles(inFname, encFname, logFname)) { // Ensure files are open
        SetRotorPositions(); // Set initial rotor positions
        ReportMachine();     // Log machine configuration
        ProcessPlainText();  // Perform encryption or decryption
        CloseFiles();        // Close all files
    }
}

void reset()
{
    for (int i = 1; i <= mRotors; ++i) {
        window[i] = Iwindow[i]; // Reset rotor positions to initial values
    }
}



void ShowRotors()
{
    int i, j, k;
    char *Rwiring;

    for (i = mRotors; i >= 1; --i) {
        fprintf(logFp, "%d: ", i);
        Rwiring = RotWiring[i];
        k = RotPos[i];
        for (j = 0; j < k; ++j) {
            fprintf(logFp, "%c", *Rwiring++);
        }
        fprintf(logFp, "->");
        for (j = k; j < Nchars; ++j) {
            fprintf(logFp, "%c", *Rwiring++);
        }
        fprintf(logFp, "\n");
    }
}





















int main()
{
    InitEnigma();      // Initialize the machine with default settings
    TryUserSetup();    // Attempt to load custom setup from "esetup"

    // Process encryption and decryption
    ProcessFile("plain", "encrypt", "elog");
    reset();           // Reset rotor positions
    ProcessFile("encrypt", "decrypt", "dlog");

    return 0;
}
