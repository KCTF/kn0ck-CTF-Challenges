#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define MAX_CHOSEN_PAIRS 10000
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

int winner = 0;
int loser = 0;

unsigned long subkey[6];//= { NULL, NULL,NULL, NULL, NULL, NULL };

unsigned char rotl2(unsigned char a) { return ((a << 2) | (a >> 6)); }

unsigned long leftHalf(unsigned long long a) { return (a >> 32LL); }
unsigned long rightHalf(unsigned long long a) { return a & 0xffffffff; }
unsigned char sepByte(unsigned long a, unsigned char index) { return a >> (8 * index); }
unsigned long combineBytes(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
	return (b3 << 24L | (b2 << 16L) | (b1 << 8L) | b0 )& 0xFFFFFFFFLL;
}
unsigned long long combineHalves(unsigned long leftHalf, unsigned long rightHalf)
{
	return (((unsigned long long)(leftHalf)) << 32LL) | (((unsigned long long)(rightHalf)) & 0xFFFFFFFFLL);
}

unsigned char gBox(unsigned char a, unsigned char b, unsigned char mode)
{
	return rotl2(a + b + mode);
}

unsigned long fBox(unsigned long plain)
{
	unsigned char x3 = sepByte(plain, 0);
	unsigned char x2 = sepByte(plain, 1);
	unsigned char x1 = sepByte(plain, 2);
	unsigned char x0 = sepByte(plain, 3);

	unsigned char t0 = (x0 ^ x3);

	unsigned char y1 = gBox(x2 ^ x1, t0, 1);
	unsigned char y0 = gBox(x1, y1, 0);
	unsigned char y2 = gBox(t0, y1, 0);
	unsigned char y3 = gBox(x0, y2, 1);

	return combineBytes(y3, y0, y1, y2);
}


void generateSubkeys(int seed)
{
	srand(seed);

	int c;
	for (c = 0; c < 6; c++)
		subkey[c] = (rand() << 16L) | (rand() & 0xFFFFL);
}

int numPlain;
unsigned long long plain0[MAX_CHOSEN_PAIRS];
unsigned long long cipher0[MAX_CHOSEN_PAIRS];
unsigned long long plain1[MAX_CHOSEN_PAIRS];
unsigned long long cipher1[MAX_CHOSEN_PAIRS];

//mark

unsigned long long statsp0[3][8] = {{0xaa1c777b385477a4,0x9e8a75338eef0150,0x218bd0cae04a9b0b,0x74f38a375332c755,0x7b2a6a482daf4965,0x8c4728c97f34847a,0x35bdaa67f7f02a88,0x964eea7ab444b0ab},{0x0bdf7ca9eca77306,0xd67b2740c1c2ebf9,0xec2cc3b2bfd84110,0x1e165f3d18950703,0x20b1ff0bf6c426b6,0x74cfdad7419a7316,0x020fc951b3cf4302,0xa25b18479c81e4da},{0x091e6fc5ed7c29b8,0x9495e9240d26a3c0,0x015235d29124d2f8,0x53f980c1994c514b,0x3e15223fd7fe09cf,0x3cf97da656b21dcd,0x8a870bf8059f50da,0x2c8b06c72b59bc33}};
unsigned long long statsc0[3][8] = {{0x5cb82d6f2055e348,0x2e2a77e4ea43f2a7,0x7b05b289bc8be2a1,0xbef2754b8814e526,0x5e98bc4f9954af75,0x96b973cf223c5d25,0xba2a1412001d4b49,0xcd87260920958762},{0x93db2f51be95851a,0x90c94e4b951c0c85,0x79404391ff3e68f8,0xbaab6b5f22c50e2a,0xa85008ebf57a594f,0x080b476b588123e9,0x84af0866e7c199b4,0xc53719a09a1f6920},{0x9ecb8818e3caf112,0x07d5face1ee43f5f,0xdb448bfadfe08b06,0x83eac21dd913e3e9,0xd5de0c5ceab685e6,0xcd13df17be22f83d,0xe5ec6a47bf68b0f2,0x3b2fc8c11412e434}};
unsigned long long statsp1[3][8] = {{0xaa9cf77b385477a4,0x9e0af5338eef0150,0x210b50cae04a9b0b,0x74730a375332c755,0x7baaea482daf4965,0x8cc7a8c97f34847a,0x353d2a67f7f02a88,0x96ce6a7ab444b0ab},{0x0b5ffca9ec27f306,0xd6fba740c1426bf9,0xecac43b2bf58c110,0x1e96df3d18158703,0x20317f0bf644a6b6,0x744f5ad7411af316,0x028f4951b34fc302,0xa2db98479c0164da},{0x091c6fc5edfea9b8,0x9497e9240da423c0,0x015035d291a652f8,0x53fb80c199ced14b,0x3e17223fd77c89cf,0x3cfb7da656309dcd,0x8a850bf8051dd0da,0x2c8906c72bdb3c33}};
unsigned long long statsc1[3][8] = {{0x896b69fb74eebffc,0x31ba4b8149bbc623,0xa610711ee1b63916,0xb694f6cd005a6e80,0xb769911ae54c8ac3,0xf2aff6a5c712c06f,0xc27296a8f83dd113,0x676753ec0f9efa66},{0x8542387400bdd855,0xa97570a510af791e,0xc2ffd7fad1d23a48,0x8d5f5be15a1cce1a,0x438ebba982d05172,0x56a6f0e87cf066af,0x7d7a67dca2ee3e6d,0xe5be7edd9165d890},{0x310c5a3852a6aa5f,0xb84067d39882eb1c,0xd73d6ea763c42475,0xb05de813ffc14798,0xd67ef5a028708766,0x94705729835fdf02,0xa80c4a6db688f27e,0xb43aeff3aaa37b50}};


unsigned long crackLastRound()
{
	printf("  Cracking...");

	unsigned long fakeK;
	for (fakeK = 0x0LL; fakeK < 0xFFFFFFFFL; fakeK++)
	{
		int score = 0;
		int c;
		for (c = 0; c < numPlain; c++)
		{
			unsigned long Y0 = leftHalf(cipher0[c]) ^ rightHalf(cipher0[c]);
			unsigned long Y1 = leftHalf(cipher1[c]) ^ rightHalf(cipher1[c]);

			unsigned long fakeInput0 = Y0 ^ fakeK;
			unsigned long fakeInput1 = Y1 ^ fakeK;
			unsigned long fakeOut0 = fBox(fakeInput0);
			unsigned long fakeOut1 = fBox(fakeInput1);
			unsigned long fakeDiff = fakeOut0 ^ fakeOut1;
			unsigned long Z = leftHalf(cipher0[c]) ^ leftHalf(cipher1[c]) ^ 0x00828000;

			if (fakeDiff == Z) score++; else break;
		}

		if (score == numPlain)
		{
			printf("found subkey : 0x%08lx\n", fakeK);
			return fakeK;
		}
		else if(score>3)
		{
			printf("score : %d\n", score);
			printf("found subfakekey : 0x%08lx\n", fakeK);
		}
	}
	printf("failed\n");
	return 0;
}

void chosenPlaintext(unsigned long long diff)
{

	int c = -1;
	if (diff == 0x0080800000000000LL) {
		c = 0;
	}
	else if (diff == 0x0080800000808000LL) {
		c = 1;
	}
	else if (diff == 0x0002000000828000LL) {
		c = 2;
	}
	for (int i = 0; i < numPlain; i++) {
		plain0[i] = statsp0[c][i];
		plain1[i] = statsp1[c][i];
		cipher0[i] = statsc0[c][i];
		cipher1[i] = statsc1[c][i];
	}
}

void undoLastRound(unsigned long crackedSubkey)
{
	int c;
	for (c = 0; c < numPlain; c++)
	{
		
		unsigned long cipherRight0 = leftHalf(cipher0[c]) ^ rightHalf(cipher0[c]);
		unsigned long cipherLeft0 = rightHalf(cipher0[c]) ^ fBox(cipherRight0 ^crackedSubkey);

		unsigned long cipherRight1 = leftHalf(cipher1[c]) ^ rightHalf(cipher1[c]);
		unsigned long cipherLeft1 = rightHalf(cipher1[c]) ^ fBox(cipherRight1 ^crackedSubkey);


		cipher0[c] = combineHalves(cipherLeft0, cipherRight0);
		cipher1[c] = combineHalves(cipherLeft1, cipherRight1);
	}
}

void doLastRound(unsigned long crackedSubkey)
{
	int c;
	for (c = 0; c < numPlain; c++)
	{

		unsigned long cipherRight0 = rightHalf(cipher0[c]);
		unsigned long cipherLeft0 = leftHalf(cipher0[c]);
		cipherLeft0 = cipherLeft0 ^ fBox(cipherRight0 ^crackedSubkey);
		unsigned long tmp = cipherLeft0;
		cipherLeft0 = cipherLeft0 ^ cipherRight0;
		cipherRight0 = tmp;

		unsigned long cipherRight1 = rightHalf(cipher1[c]);
		unsigned long cipherLeft1 = leftHalf(cipher1[c]);
		cipherLeft1 = cipherLeft1 ^ fBox(cipherRight1 ^crackedSubkey);
		tmp = cipherLeft1;
		cipherLeft1 = cipherLeft1 ^ cipherRight1;
		cipherRight1 = tmp;

		cipher0[c] = combineHalves(cipherLeft0, cipherRight0);
		cipher1[c] = combineHalves(cipherLeft1, cipherRight1);
	}
}

void prepForCrackingK0()
{
	int c;
	for (c = 0; c < numPlain; c++)
	{
		unsigned long cipherLeft0 = leftHalf(cipher0[c]);
		unsigned long cipherRight0 = rightHalf(cipher0[c]);
		unsigned long cipherLeft1 = leftHalf(cipher1[c]);
		unsigned long cipherRight1 = rightHalf(cipher1[c]);

		unsigned long tempLeft0 = cipherLeft0;
		unsigned long tempLeft1 = cipherLeft1;
		cipherLeft0 = cipherRight0;
		cipherLeft1 = cipherRight1;
		cipherRight0 = tempLeft0;
		cipherRight1 = tempLeft1;

		cipher0[c] = combineHalves(cipherLeft0, cipherRight0);
		cipher1[c] = combineHalves(cipherLeft1, cipherRight1);
	}
}

int main()
{
	printf("refer to JK'S FEAL-4 DIFFERENTIAL CRYPTANALYSIS DEMO\n");
	printf("-------------------------------------------\n");
	printf("\n");
	int graphData[20];

	int c;

	numPlain = 8;
	unsigned long long inputDiff1 = 0x0080800000000000LL;
	unsigned long long inputDiff2 = 0x0080800000808000LL;
	unsigned long long inputDiff3 = 0x0002000000828000LL;
	//unsigned long outDiff = 0x00020000L;

	unsigned long fullStartTime = time(NULL);

	//CRACKING ROUND 4
	printf("ROUND 4\n");
	chosenPlaintext(inputDiff1);
	unsigned long startTime = time(NULL);
	unsigned long crackedSubkey3;
	if (subkey[3] == NULL)
		crackedSubkey3 = crackLastRound();
	else
		crackedSubkey3 = subkey[3];
	unsigned long endTime = time(NULL);
	printf("  Time to crack round #4 = %i seconds\n", (int)(endTime - startTime));

	//CRACKING ROUND 3
	printf("ROUND 3\n");
	chosenPlaintext(inputDiff2);
	undoLastRound(crackedSubkey3);
	startTime = time(NULL);
	unsigned long crackedSubkey2;
	if (subkey[2] == NULL)
		crackedSubkey2 = crackLastRound();
	else
		crackedSubkey2 = subkey[2];
	endTime = time(NULL);
	printf("  Time to crack round #3 = %i seconds\n", (int)(endTime - startTime));

	//CRACKING ROUND 2
	printf("ROUND 2\n");
	chosenPlaintext(inputDiff3);
	undoLastRound(crackedSubkey3);
	undoLastRound(crackedSubkey2);
	startTime = time(NULL);
	unsigned long crackedSubkey1;
	if (subkey[1] == NULL)
		crackedSubkey1 = crackLastRound();
	else
		crackedSubkey1 = subkey[1];
	endTime = time(NULL);
	printf("  Time to crack round #2 = %i seconds\n", (int)(endTime - startTime));

	//CRACK ROUND 1
	printf("ROUND 1\n");
	undoLastRound(crackedSubkey1);
	startTime = time(NULL);
	unsigned long crackedSubkey0 ;
	unsigned long crackedSubkey4 ;
	unsigned long crackedSubkey5 ;
	startTime = time(NULL);
	unsigned long fakeK;
	unsigned long guessK;
	for (fakeK = 0x00000000L; fakeK < 0xFFFFFFFFL; fakeK++)
	{
		int score = 0;
		int c;
		for (c = 0; c < numPlain; c++)
		{
			unsigned long Y0 = leftHalf(cipher0[c]) ^ rightHalf(cipher0[c]);
			unsigned long Y1 = leftHalf(cipher1[c]) ^ rightHalf(cipher1[c]);

			unsigned long fakeInput0 = Y0 ^ fakeK;
			unsigned long fakeInput1 = Y1 ^ fakeK;
			unsigned long fakeOut0 = fBox(fakeInput0);
			unsigned long fakeOut1 = fBox(fakeInput1);
			unsigned long fakeDiff = fakeOut0 ^ fakeOut1;
			unsigned long Z = rightHalf(cipher0[c]) ^ rightHalf(cipher1[c]) ^ 0x00020000;

			if (fakeDiff == Z) score++; else break;
		}

		if (score == numPlain)
		{
			crackedSubkey0 = fakeK;
			//crack crackedSubkey4 crackedSubkey5 and check 
			undoLastRound(crackedSubkey0);
			crackedSubkey4 = leftHalf(cipher0[0]) ^ leftHalf(plain0[0]);
			crackedSubkey5 = rightHalf(cipher0[0]) ^ rightHalf(plain0[0]);
			int score = 0;
			for (c = 0; c < numPlain; c++)
			{
				unsigned long cracked4 = leftHalf(cipher0[c]) ^ leftHalf(plain0[c]);
				unsigned long cracked41 = leftHalf(cipher1[c]) ^ leftHalf(plain1[c]);
				unsigned long cracked5 = rightHalf(cipher0[c]) ^ rightHalf(plain0[c]);
				unsigned long cracked51 = rightHalf(cipher1[c]) ^ rightHalf(plain1[c]);

				if ((cracked4 == crackedSubkey4) && 
					(cracked41 == crackedSubkey4) &&
					(cracked5 == crackedSubkey5) &&
					(cracked51 == crackedSubkey5) 
					)
					score++;
			}

			if (score == numPlain)
			{
				printf("found subkeys : 0x%08lx  0x%08lx  0x%08lx\n", crackedSubkey0, crackedSubkey4, crackedSubkey5);
				break;
				doLastRound(crackedSubkey0);
			}
			else
			{
				doLastRound(crackedSubkey0);
				printf("failed.score:%d\n", score);
			}

		}
	}
	endTime = time(NULL);
	printf("  Time to crack round #1 = %i seconds\n", (int)(endTime - startTime));


	unsigned long fullEndTime = time(NULL);
	printf("Total crack time = %i seconds\n", (int)(fullEndTime - fullStartTime));
	printf("FINISHED\n");
    printf("subkey = [ 0x%08lx , 0x%08lx , 0x%08lx, 0x%08lx, 0x%08lx, 0x%08lx]\n", crackedSubkey0, crackedSubkey1, crackedSubkey2, crackedSubkey3, crackedSubkey4, crackedSubkey5);
	

	return 0;
}