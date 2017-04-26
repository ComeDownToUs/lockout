Will likely redo this from scratch at some point in the near future with a different central algorithm.


# Table of Contents
1. [What is Lockout](#what-is-lockout)
  * [Okay... why?](#okay-why)
    * [Case Study: Facebook](#case-study-facebook)
2. [How it works](#how-it-works)
  * [Encryption](#encryption)
  * [Decryption](#decryption)
3. [Next Steps](#next-steps)
  * [Pipe Dreams](#pipe-dreams)
4. [How to Run -- Current Testing Process](#how-to-run)
5. [FAQ](#faq)
6. [Other People's Code](#credit)


## What is Lockout

Lockout is a prototype which is designed to place a barrier between the user and data which they have encrypted by scrambling a predetermined portion of the key.

### Okay... why?

The internet is addictive (wow, right?).
Deleting a user account can be a pretty major step to take, sacrificing all of whatever investment you have put into it. While many websites offer a softer deactivation option for people unwilling to do that, this is often as easy to reactivate as regularly logging in, which serves to make it incredibly easy to get drawn back in again.

#### Case Study: Facebook 
To delete ones account, there is a 14 day grace period where it is effectively just deactivated. That's a pretty long timespan to either (a) change your mind and get back into it or (b) decide that deletion is too big of a step and opt to deactivate the profile instead.
When deactivating a profile, the barrier to returning is practically nonexistant, just enter your login credentials and resume as before. Maybe there's some slight social shame if you make a big deal out of quitting.

By changing your password to one generated via this program (and, in the case of facebook, removing the easier account recovery options), you can put a significant time barrier between deciding to access your account and actually being able to.

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
3. Formal command line argument schema and some general security tests. 
4. Separate password prompt implementation unless specified in command
5. Simple command-line user interface

### Pipe dreams
- a client side Javascript implementaion (likely using a faster algorithm)
- self-coded implementation of AES
- this actually being as useful as I hope

## Current Test
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


## FAQs
For want of a better word... but yes(!), I'd love to hear some questions and feedback. Right now I'll just run through what I can think someone may ask. I'll just add content here as I think of it for now.

### Why the use of unsigned 8 bit integers (uint8_t) instead of unsigned characters?
The primary reason is kokke's AES implementation uses them and I liked the consistency https://github.com/kokke/tiny-AES128-C
A secondary reason is, as I opted to work around dealing directly with bit level changes and have no desire to make the system use more than an 8-bit character set, I found it pretty helpful as a reinforcement that I would dealing with a series of bytes rather than a string. This system uses my usage pretty liberally, treating them as unsigned char variables all over the shop without explicit casting. 

# Credit

This repo utilises other people's code in the following sources:
- `aes.c` and `aes.h` use TINY AES128 C by kokke from https://github.com/kokke/tiny-AES128-C
- `sha256.c` and `sha256.h` use B-Con's implementations https://github.com/B-Con/crypto-algorithms
If there are any issues with these inclusions, they will be replaced with alternatives shortly after I have been notified. I've made a point of not meddling with this code much.

