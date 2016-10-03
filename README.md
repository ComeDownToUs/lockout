### 

This repo utilises code in the following sources:
- `aes.c` and `aes.h` use TINY AES128 C from https://github.com/kokke/tiny-AES128-C
- `sha256.c` and `sha256.h` use https://github.com/B-Con/crypto-algorithms
If there are any issues with these inclusions, they will be replaced with alternatives shortly after I have been notified.


## What is Lockout

Lockout is a prototype which is designed to place a barrier between the user and data which they have encrypted by scrambling a predetermined portion of the key.

## How it works

### Encryption
The user provides the following inputs
- plaintext (PT)
- user password (PW)
- decryption strength (DS)
- a salt word (optional) (S)

1. PW is passed through a hash algorithm
2. the first X bits (currently 128) of h(PW) are subsequently passed into a key generator along with DS
3. The key generator maintains the first DS of h(PW) and replaces the remaining 128 - DS bit with randomised values to create the encryption key K *
4. The plaintext is appended with either the optional S or PW if no S was given
5. The plaintext is encrypted using K, all other inputted values are forgotten unless saved elsewhere

* //note: it currently maintains the first DS/8 ~bytes~ and limits the next byte to a figure which ensures the brute force algorithm will resolve its value within the desired level of complexity. e.g. An argument of 127 will keep the first remaining 15 bytes of a 16 byte (or 128 bit) key as is, the final byte will be limited to a value of range 2^1 values.
I do plan on eventually writing up all of these portions using bitwise operators to get a bit better with them, but I don't want to wreck my head with that for now.

### Decryption
The user provides:
- ciphtertext (CT)
- user password (PW)
- salt word (optional) (S)

An initial test key is built using h(PW)
A recursive function attempts to decrypt the ciphertext using all possible key combinations, beginning by changing the final bit and working its way back. Following each attempt, a check is made to see if the appended value from step 4 is present, in which case it has successfully decrypted the data.

If the user has retained the value of DS when decrypting, they should know the maximum number of attempts needed and therefore also be able to eventually tell whether they attempt was unsuccessful.


## Next steps
Currently at the most basic acceptable stage, the next steps for this project are:

1. A process to read in and write to files
2. Timing tests added to debugging to remove significant inefficiencies (only significant ones, it's beyond my scope to make this run perfectly)
3. Formal command line argument schema and some general security tests
4. simple user interface


### Current Test
To run the current, very basic, test implementation:
Compile with 
```
gcc -o tester test.c aes.c sha256.c lockout.c
```
To run
```
./tester [password] [user inputted string argument] [decryption difficulty]
```
- Due to some lazy testing, the password currently needs to be exactly 8 characters
- User inputted string is currently set to a limit of 32 bytes, just change the TEXTSIZE constant to make it larger. Will make this flexible at some point.
- decryption difficulty currently specifies how many entries of the key are NOT to be randomised (i.e. 128 means all 128 bit are to remain as is). I recommend leaving this setting at a figure over 112 until you decide which outputs you wish to leave in.