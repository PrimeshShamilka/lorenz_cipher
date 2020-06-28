#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum Shift
{
    LTRS = 31,
    FIGS = 27
} Shift;

typedef struct ascii2_baudot

{
    char code;
    Shift shift;
} ascii2_baudot;

Shift state = LTRS;

// wheel settings for the Lorenz cipher
char chi1[41] = {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1};
char chi2[31] = {1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0};
char chi3[29] = {1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0};
char chi4[26] = {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1};
char chi5[23] = {1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0};

char psi1[43] = {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0};
char psi2[47] = {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0};
char psi3[51] = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1};
char psi4[53] = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1};
char psi5[59] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0};
char mu1[61] = {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0};
char mu2[37] = {0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};


ascii2_baudot ascii2_baudot[128] =
{
    ['\0'] = {'\0', FIGS},	['3'] = {0x01, FIGS},	['\n'] = {0x02, FIGS},
    ['-'] = {0x03, FIGS},	[' '] = {0x04, FIGS},	['\''] = {0x05, FIGS},
    ['8'] = {0x06, FIGS},	['7'] = {0x07, FIGS},	['\r'] = {0x08, FIGS},
    [0x05] = {0x09, FIGS},	['4'] = {0x0b, FIGS},	['\a'] = {0x0b, FIGS},
    [','] = {0x0c, FIGS},	['!'] = {0x0d, FIGS},	[':'] = {0x0e, FIGS},
    ['('] = {0x0f, FIGS},	['5'] = {0x10, FIGS},	['+'] = {0x11, FIGS},
    [')'] = {0x12, FIGS},	['2'] = {0x13, FIGS},	['$'] = {0x14, FIGS},
    ['6'] = {0x15, FIGS},	['0'] = {0x16, FIGS},	['1'] = {0x17, FIGS},
    ['9'] = {0x18, FIGS},	['?'] = {0x19, FIGS},	['&'] = {0x1a, FIGS},
    ['.'] = {0x1c, FIGS},	['/'] = {0x1d, FIGS},	[';'] = {0x1e, FIGS},
    ['\0'] = {'\0', LTRS},	['E'] = {0x01, LTRS},	['\n'] = {0x02, LTRS},
    ['A'] = {0x03, LTRS},	[' '] = {0x04, LTRS},	['S'] = {0x05, LTRS},
    ['I'] = {0x06, LTRS},	['U'] = {0x07, LTRS},	['\r'] = {0x08, LTRS},
    ['D'] = {0x09, LTRS},	['R'] = {0x0a, LTRS},	['J'] = {0x0b, LTRS},
    ['N'] = {0x0c, LTRS},	['F'] = {0x0d, LTRS},	['C'] = {0x0e, LTRS},
    ['K'] = {0x0f, LTRS},	['T'] = {0x10, LTRS},	['Z'] = {0x11, LTRS},
    ['L'] = {0x12, LTRS},	['W'] = {0x13, LTRS},	['H'] = {0x14, LTRS},
    ['Y'] = {0x15, LTRS},	['P'] = {0x16, LTRS},	['Q'] = {0x17, LTRS},
    ['O'] = {0x18, LTRS},	['B'] = {0x19, LTRS},	['G'] = {0x1a, LTRS},
    ['M'] = {0x1c, LTRS},	['X'] = {0x1d, LTRS},	['V'] = {0x1e, LTRS}
};

const char *to_binary2[32] =
{
    [0x00] = "00000",	[0x01] = "00001",	[0x02] = "00010",	[0x03] = "00011",	[0x04] = "00100",
    [0x05] = "00101",	[0x06] = "00110",	[0x07] = "00111",	[0x08] = "01000",	[0x09] = "01001",
    [0x0a] = "01010",	[0x0b] = "01011",	[0x0c] = "01100",	[0x0d] = "01101",	[0x0e] = "01110",
    [0x0f] = "01111",	[0x10] = "10000",	[0x11] = "10001",	[0x12] = "10010",	[0x13] = "10011",
    [0x14] = "10100",	[0x15] = "10101",	[0x16] = "10110",	[0x17] = "10111",	[0x18] = "11000",
    [0x19] = "11001",	[0x1a] = "11010",	[0x1b] = "11011",   [0x1c] = "11100",	[0x1d] = "11101",
    [0x1e] = "11110",   [0x1f] = "11111"
};

unsigned char *encode_ita2(char *s)
{
    unsigned char *str = malloc((strlen(s) * 2) + 1);
    unsigned char *ptr = str;

    int i = 0;
    reset_shift();
    while (*s)
    {
        char c = *s;
        if (islower(c))
            c = toupper(c);
        ascii2_baudot ch = ascii2_baudot[(int)c];
        if (state != ch.shift)
        {
            if (state == LTRS)
            {
                *ptr++ = 0x1b;
            }
            else
            {
                *ptr++ = 0x1f;
            }
            state = ch.shift;
        }
        *ptr++ = ch.code;
        s++;
        i++;
    }
    *ptr = '\0';
    return str;
}


// wheel rotation (shift register operation)
void move_chi_pos(int chi_pos[5])
{
    chi_pos[0] = (chi_pos[0] + 1) % 41;
    chi_pos[1] = (chi_pos[1] + 1) % 31;
    chi_pos[2] = (chi_pos[2] + 1) % 29;
    chi_pos[3] = (chi_pos[3] + 1) % 26;
    chi_pos[4] = (chi_pos[4] + 1) % 23;
}

void move_psi_pos(int psi_pos[5])
{
    psi_pos[0] = (psi_pos[0] + 1) % 43;
    psi_pos[1] = (psi_pos[1] + 1) % 47;
    psi_pos[2] = (psi_pos[2] + 1) % 51;
    psi_pos[3] = (psi_pos[3] + 1) % 53;
    psi_pos[4] = (psi_pos[4] + 1) % 59;
}

void move_mu_pos1(int *mu_pos1)
{
    *mu_pos1 = (*mu_pos1 + 1) % 61;
}

void move_mu_pos2(int *mu_pos2)
{
    *mu_pos2 = (*mu_pos2 + 1) % 37;
}

// utility functions
void print_chi(int chi_pos[5])
{
    printf("%i ", chi1[chi_pos[0]]);
    printf("%i ", chi2[chi_pos[1]]);
    printf("%i ", chi3[chi_pos[2]]);
    printf("%i ", chi4[chi_pos[3]]);
    printf("%i ", chi5[chi_pos[4]]);
    printf("| ");
}
void print_psi(int psi_pos[5])
{
    printf("%i ", psi1[psi_pos[0]]);
    printf("%i ", psi2[psi_pos[1]]);
    printf("%i ", psi3[psi_pos[2]]);
    printf("%i ", psi4[psi_pos[3]]);
    printf("%i ", psi5[psi_pos[4]]);
    printf("| ");
}

void print_mu(int mu_pos[2])
{
    printf("%i ", mu1[mu_pos[0]]);
    printf("%i ", mu2[mu_pos[1]]);
    printf("| ");
}

void reset_shift()
{
    state = LTRS;
}

void helper(unsigned char *chr)
{

    char K[5];
    char clock_mu_wheel;
    char clock_psi_wheel;

    // Starting positions of wheels
    int chi_pos[5] = {0, 0, 0, 0, 0};
    int psi_pos[5] = {0, 0, 0, 0, 0};
    int mu_pos[2] = {0, 0};

    int changed_charactor_count = 0;
    int charactor_count = 0;
    int encrypted_charactor_count = 0;

    while (*chr)
    {
        //wheel turn
        K[0] = chi1[chi_pos[0]] ^ psi1[psi_pos[0]];
        K[1] = chi2[chi_pos[1]] ^ psi2[psi_pos[1]];
        K[2] = chi3[chi_pos[2]] ^ psi3[psi_pos[2]];
        K[3] = chi4[chi_pos[3]] ^ psi4[psi_pos[3]];
        K[4] = chi5[chi_pos[4]] ^ psi5[psi_pos[4]];
        clock_mu_wheel = 0;
        clock_psi_wheel = 0;
        move_chi_pos(chi_pos);

        move_mu_pos1(&mu_pos[0]);
        if (mu1[mu_pos[0]] == 1)
            clock_mu_wheel = 1;
        if (clock_mu_wheel == 1)
        {
            move_mu_pos2(&mu_pos[1]);
        }

        if (mu2[mu_pos[1]] == 1)
            clock_psi_wheel = 1;
        if (clock_psi_wheel == 1)
            move_psi_pos(psi_pos);

        //Generate baudot code from ascii code
        char c = *chr;
        unsigned char f;
        if (islower(c))
            c = toupper(c);
        ascii2_baudot ch = ascii2_baudot[(int)c];

        //add the shift chnage to ciphertext
        if (state != ch.shift)
        {
            if (state == LTRS)
            {

                const char *arr1 = to_binary2[0x1f];
                char OUT1[5];
                OUT1[0] = K[0] ^ arr1[0];
                OUT1[1] = K[1] ^ arr1[1];
                OUT1[2] = K[2] ^ arr1[2];
                OUT1[3] = K[3] ^ arr1[3];
                OUT1[4] = K[4] ^ arr1[4];
                // printf("%s\n", OUT1);
            }
            else
            {
                const char *arr2 = to_binary2[0x1b];
                char OUT2[5];
                OUT2[0] = K[0] ^ arr2[0];
                OUT2[1] = K[1] ^ arr2[1];
                OUT2[2] = K[2] ^ arr2[2];
                OUT2[3] = K[3] ^ arr2[3];
                OUT2[4] = K[4] ^ arr2[4];
                // printf("%s\n", OUT2);
            }
            state = ch.shift;
            encrypted_charactor_count++;
        }
        const char *arr3 = to_binary2[ch.code];
        char OUT3[5];
        //xor wheel generated 5 bit with original 5 bit
        OUT3[0] = K[0] ^ arr3[0];
        OUT3[1] = K[1] ^ arr3[1];
        OUT3[2] = K[2] ^ arr3[2];
        OUT3[3] = K[3] ^ arr3[3];
        OUT3[4] = K[4] ^ arr3[4];
        // printf("output :%s\n", OUT3);
        // printf("input :%s\n", arr3);

        //bitwise comapre for calculate number of bits from the input plaintext that was changed in the ciphertext
        int count=0;
        for(int i=0; i<5; i++)
        {
            if(arr3[i]!=OUT3[i])
            {
                count++;
            }
        }
        changed_charactor_count+=count;


        chr++;
        charactor_count++;
        encrypted_charactor_count++;
    }

    printf("Total charactor count: %i\n",charactor_count);
    printf("Encrypted charactor count: %i\n",encrypted_charactor_count);
    printf("Changed charactor count: %i\n",changed_charactor_count);
    charactor_count*=5;
    float i = (float)changed_charactor_count/(float)charactor_count;
    printf("Percentage of changed bits in the ciphertext compared to plaintext: %f%%\n",i*100);
}

int main()
{
    char *orig_msg = "The post has just arrived and in it a very nice surprise, the discovery that Jacques Seguela, one-time adviser to President Mitterrand, now close confidant of President and Madame Sarkozy (indeed he intoduced them), and something of a legend in French political communications, has dedicated his latest book to little old moi. With apologies for the missing accents here and in the French bits of the long posting which follows – the dedication to ‘Le Pouvoir dans la Peau‘ (Power in the skin) reads ‘A Alastair Campbell, mon spin doctor prefere’ (three missing accents in one word – mes excuses sinceres). So what did I do for this honour, you are asking? Well, perhaps the fact that he asked me to read his book, and write a ‘postface’ assessment both of his writing and of the issues he covers, and the fact that I said yes, has something to do with it. He says some blushmakingly kind things in his ‘preface to the postface’, which I will have to leave to French readers of the whole thing (published by Plon). But for the largely Anglophone visitors of this blog, I thought some of you might like to read the said ‘postface’ in English (apart from the bits where I quote direct from his book). I hope all those students who write asking for help with dissertations will find something quotable in it. Meanwhile I am off to Norway for a conference and a meeting with the Norwegian Labour Party. I’m looking forward to being in the country with the highest ‘human development index’ in the world, and which showed such a mature response to the recent massacre of Oslo and Utoya. Jacques Seguela writes about political campaigns and communications not merely as an expert analyst, but as an experienced practitioner. Hence his latest book contains both insights worth heeding, but also enlivening tales of his own experience. He is observer and participant; outsider looking in, and insider looking out.  There is much to look at, not least in France with a Presidential election looming, and the outcome far from easy to predict. We live in a world defined by the pace of change, and whilst the velocity of that change has not always impacted upon our political institutions, many of which would remain recognisable to figures of history, it most certainly has impacted upon political communications. As Seguela writes: ‘En 5 ans le monde de la communication a plus evolue que dans les cents dernieres annees. ‘ Google, Youtube, Twitter, Facebook have quickly entered our language and changed the way we communicate, live our private lives, do business, do politics. People do not believe politicians as much as they once did. Nor do they believe the media. So who do we believe? We believe each other. The power and the political potential of social networks flows from that reality. Though fiercely modern in their application, social networks in some ways take us back to the politics of the village square. They are an electronic word of mouth on a sometimes global scale. This has changed the way people interact with each other and with their politicians. My first campaign as spokesman and strategist for Tony Blair was in 1997, three years in the planning after he had become leader of the Opposition  Labour Party. Some of the principles of strategy we applied back then would certainly apply to a modern day election. But their tactical execution almost certainly would not. Politicians and their strategists have to adapt to change as well as lead it. Seguela gives some interesting insights into those who have adapted well, and those who have done less well. He clearly adores former President Lula of Brazil and you can feel his yearning for a French leader who can somehow combine hard-headed strategy with human empathy in the same way as a man who left office with satisfaction ratings of 87percent. Seguela probably remains best known in political circles for his role advising Francois Mitterrand. Yet wheras I am ‘tribal Labour’, and could not imagine supporting a Conservative Party candidate in the UK, Seguela came out as a major supporter of Nicolas Sarkozy. I wonder if one of the reasons was not a frustration that large parts of the left in France remain eternally suspicious of modern communications techniques and styles which, frankly, no modern leader in a modern democracy can ignore. How he or she adapts to, or uses, them is up to them. But you cannot stand aside and imagine the world has not changed. If Lula is a star of this book, so too is Barack Obama. American elections are of enormous interest to all political campaign junkies, a category in which both Seguela and I would almost certainly qualify. Much is made of Obama’s use of the internet, a relatively new phenomenon in historical terms and one the young Senator used brilliantly in his quest to become President. Yet though it was an accurate expression of his modernity, underpinning its use were some very old-fashioned campaign principles. He used it to turn supporters into activists who both gave funds and also took his campaign materials and ideas and ran their own campaigns for him. Somehow he managed to make one of the most professional, most disciplined and best funded campaigns in history look like an enormous act of democratic participation. It was less command and control – the model we certainly adopted in 1997 and 2001, Labour’s two landslide victories, easing off a little for our third win in 2005 – than ‘inspire and empower.’  ‘Yes we can’ not ‘yes I can’. His supporters were more than supporters. They were an active part of the campaign, and of the message. The key to this was something that had nothing to do with politicians and everything to do with science, technology and the internet. Ask me who has had the most influence on campaigns in recent times and I might be tempted to reply Tim Berners-Lee, the man credited with gifting the web to the world. Its implications have been far reaching in virtually all aspects of our lives, politics and political campaigns foremost. The new household brand names of the cyber era have not replaced good policy work, messaging and organisation. But they have become essential components of the execution of them in the campaign. Mainstream conventional media remains important and influential, not least because, bizarrely, in most democracies the broadcasters continue to let the press set their agenda for them. But a candidate who tries to stand against the tide of new media will be making a big mistake, and missing big opportunities. If it has changed so much in the last five years, how much more will it change in the next five years?";
    helper(orig_msg); 
   
}

// TEXT

// The post has just arrived and in it a very nice surprise, the discovery that Jacques Seguela, one-time adviser to President Mitterrand, now close confidant of President and Madame Sarkozy (indeed he intoduced them), and something of a legend in French political communications, has dedicated his latest book to little old moi. With apologies for the missing accents here and in the French bits of the long posting which follows – the dedication to ‘Le Pouvoir dans la Peau‘ (Power in the skin) reads ‘A Alastair Campbell, mon spin doctor prefere’ (three missing accents in one word – mes excuses sinceres). So what did I do for this honour, you are asking? Well, perhaps the fact that he asked me to read his book, and write a ‘postface’ assessment both of his writing and of the issues he covers, and the fact that I said yes, has something to do with it. He says some blushmakingly kind things in his ‘preface to the postface’, which I will have to leave to French readers of the whole thing (published by Plon). But for the largely Anglophone visitors of this blog, I thought some of you might like to read the said ‘postface’ in English (apart from the bits where I quote direct from his book). I hope all those students who write asking for help with dissertations will find something quotable in it. Meanwhile I am off to Norway for a conference and a meeting with the Norwegian Labour Party. I’m looking forward to being in the country with the highest ‘human development index’ in the world, and which showed such a mature response to the recent massacre of Oslo and Utoya. Jacques Seguela writes about political campaigns and communications not merely as an expert analyst, but as an experienced practitioner. Hence his latest book contains both insights worth heeding, but also enlivening tales of his own experience. He is observer and participant; outsider looking in, and insider looking out.  There is much to look at, not least in France with a Presidential election looming, and the outcome far from easy to predict. We live in a world defined by the pace of change, and whilst the velocity of that change has not always impacted upon our political institutions, many of which would remain recognisable to figures of history, it most certainly has impacted upon political communications. As Seguela writes: ‘En 5 ans le monde de la communication a plus evolue que dans les cents dernieres annees. ‘ Google, Youtube, Twitter, Facebook have quickly entered our language and changed the way we communicate, live our private lives, do business, do politics. People do not believe politicians as much as they once did. Nor do they believe the media. So who do we believe? We believe each other. The power and the political potential of social networks flows from that reality. Though fiercely modern in their application, social networks in some ways take us back to the politics of the village square. They are an electronic word of mouth on a sometimes global scale. This has changed the way people interact with each other and with their politicians. My first campaign as spokesman and strategist for Tony Blair was in 1997, three years in the planning after he had become leader of the Opposition  Labour Party. Some of the principles of strategy we applied back then would certainly apply to a modern day election. But their tactical execution almost certainly would not. Politicians and their strategists have to adapt to change as well as lead it. Seguela gives some interesting insights into those who have adapted well, and those who have done less well. He clearly adores former President Lula of Brazil and you can feel his yearning for a French leader who can somehow combine hard-headed strategy with human empathy in the same way as a man who left office with satisfaction ratings of 87percent. Seguela probably remains best known in political circles for his role advising Francois Mitterrand. Yet wheras I am ‘tribal Labour’, and could not imagine supporting a Conservative Party candidate in the UK, Seguela came out as a major supporter of Nicolas Sarkozy. I wonder if one of the reasons was not a frustration that large parts of the left in France remain eternally suspicious of modern communications techniques and styles which, frankly, no modern leader in a modern democracy can ignore. How he or she adapts to, or uses, them is up to them. But you cannot stand aside and imagine the world has not changed. If Lula is a star of this book, so too is Barack Obama. American elections are of enormous interest to all political campaign junkies, a category in which both Seguela and I would almost certainly qualify. Much is made of Obama’s use of the internet, a relatively new phenomenon in historical terms and one the young Senator used brilliantly in his quest to become President. Yet though it was an accurate expression of his modernity, underpinning its use were some very old-fashioned campaign principles. He used it to turn supporters into activists who both gave funds and also took his campaign materials and ideas and ran their own campaigns for him. Somehow he managed to make one of the most professional, most disciplined and best funded campaigns in history look like an enormous act of democratic participation. It was less command and control – the model we certainly adopted in 1997 and 2001, Labour’s two landslide victories, easing off a little for our third win in 2005 – than ‘inspire and empower.’  ‘Yes we can’ not ‘yes I can’. His supporters were more than supporters. They were an active part of the campaign, and of the message. The key to this was something that had nothing to do with politicians and everything to do with science, technology and the internet. Ask me who has had the most influence on campaigns in recent times and I might be tempted to reply Tim Berners-Lee, the man credited with gifting the web to the world. Its implications have been far reaching in virtually all aspects of our lives, politics and political campaigns foremost. The new household brand names of the cyber era have not replaced good policy work, messaging and organisation. But they have become essential components of the execution of them in the campaign. Mainstream conventional media remains important and influential, not least because, bizarrely, in most democracies the broadcasters continue to let the press set their agenda for them. But a candidate who tries to stand against the tide of new media will be making a big mistake, and missing big opportunities. If it has changed so much in the last five years, how much more will it change in the next five years?

// Percentage of changed bits in the ciphertext compared to plaintext: 49.821831%