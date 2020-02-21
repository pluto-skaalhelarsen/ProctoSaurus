### Dangerous Diary - A Proctosurus Use Case and Scripted Solution

I recognize that this is a lot of detailed description spread out over many
small, disjointed programs that represent several different conceptual
viewpoints and potential use cases. Especially for those who find themselves
still interested in some aspect of this after an admittedly scattered
exposition - I feel your pain. Addressing that difficulty, I've included
here a script and a lightly commented output from the script that steps
through some simple examples. This is an example intended to show a somewhat
plausible situation and how the Thesaurus of the Gods and ProctoSaurus tools
might be used to address it.

### The Premise

Imagine that you are a lowly servant in the bookkeeping office of a small
firm that does much of its business with much larger and more powerful
organizations than your employer. Not as large and powerful as our
government, of course, since they are beyond reproach. Perhaps you've seen
in the course of your work several unsettling details that might or might
not indicate embezzlement or other financial malfeasance. Taking the issue
up the chain of command could threaten your employment and put your three
small children with yourself on the street among the millions. You decide
to keep a diary of your suspicions until such time as you have accumulated
the evidence necessary to act decisively.

So, the diary entries are short but contain specific details such as names,
dates, amounts check numbers, etc. You have a laptop at your job, but many
people have access to it. You posses the future industrial version of the
Thesaurus of the Gods and ProctoSaurus tools that will be available in our
near-future sci-fi story. We’ll use the actual tools in this repository as
a stand-in. They are running on a thumb drive with a data directory full of
personal email, drafts of messages, laundry and grocery lists. Many people
have seen this directory and no one cares.

### The Approach

You have decided to use one of the many drafts from an email conversation
about next year’s family reunion as the template for your diary files.
This file has a date that’s easy to remember so you won’t mistake it for
any other. That text contains the names of many people, places and food
items, so you make a saurus from the text lists

>FemaleFirstNames.txt
>MaleFirstNames.txt
>LastNames.txt
>UsCityNames.txt
>Grocery.txt

Which you put in the file ProctoDemo.FileList.txt in preparation for
creation of a saurus file using the Thesaurus of the Gods ListTool.c code.
You edit the list of operations the tool performs after calling the usual
multi-file load to create the following sequence

```
SetAllWordsToLowerCase(SaurusArray, SaurusArrayCnt, WordArray, WordArrayCnt);

RemoveAllMultiwordEntries(WordArray, WordArrayCnt);

ReindexSaurus(SaurusArray, SaurusArrayCnt, WordArray, &WordArrayCnt);

RepeatedItemResolve(SaurusArray, SaurusArrayCnt, WordArray, &WordArrayCnt);

RemoveAllSingletonEntries(SaurusArray, SaurusArrayCnt);

	// personal names only
for(i=0; i < 3; i++)
    RandomizeIndexEntries(SaurusArray[i].HeadIndex, SaurusArray[i].HeadIndexCnt);

for(i=0; i < 3; i++)	// personal names only
    XformIndexToFullPartition(&SaurusArray[i]);

	// update the word structures to reflect the new index locations
ReindexSaurus(SaurusArray, SaurusArrayCnt, WordArray, &WordArrayCnt);

SaveAllToMultiIndexFile(SaurusArray, SaurusArrayCnt, "ProctoDemo.saurus.txt");

```
Then you run the list tool to create that file. It shifts everything to
lower case, eliminates repeated entries and shared entries that may occur
between files. It randomizes the alphabetized names and makes the first and
last names into better organized lists with maximally diverse list lengths.

### Using the ProctoSaurus Tools

With the ProctoDemo.saurus.txt saurus file prepared, and the file
ProctoDemo.template.txt selected as the template file, you use the TotG
debug version to generate a chain:

./Totg_dbg -rw ProctoDemo.saurus.txt ProctoDemo.template.txt > ProctoDemo.template.chain.txt

Then you take a look at the capacity of the template.

./CalcCapacity -p ProctoDemo.template.chain.txt

Loading chain file ProctoDemo.template.chain.txt
ChainLen 222
Chain capacity:
3420380687929303142976302567254444055291373009367007370962797324285021711098773
1008151383820294268902184352296811286758331527118260736490194457796782384959879
9680538230822567541456302181978236041514286012901961923358968160202846683586419
8393261573971432497082059980800000000000000000000000000000000000000000000000000
0000000000000000000
    (1112 bits)

That is the numeric value of the combined product of all the entropies in the
chain. That is one larger than the largest number that can be represented in
the chain, so the largest message that can be encoded numerically with that
template.

Now you want to know what the size of a typical entry in the journal
recordings you want to make. The text of the first entry must be converted
into a single numeric value. The text:

June 6, 2028 Check #14732 to Bumpy Trucking and Garbage Collection,
annotation: services rendered 4/15. Hand delivered to Bumpy’s office by
John Shaft.

You put that on the command line and execute:

./TextToNum “June 6, 2028 Check #14732 to Bumpy Trucking and Garbage Collection, annotation: services rendered 4/15. Hand delivered to Bumpys office by John Shaft.”
4421547151858877364227208750908738620733607601859739372166778341524785742550807474034193837670295505831104819159095432207230080492609913152702863057013041792095154406751851568447861921620672338622912730786271117430703079263978347455540995607801300412823380959215323585415884495086046257201591510925531421584710679242

Which is a smaller number. Being particularly careful that no one can read
this, you decide to combine it with a random number. How many digits should
that be? Re-execute the TextToNum command with the -p option:

./TextToNum -p "June 6, 2028 Check #14732 to Bumpy Trucking and Garbage Collection, annotation: services rendered 4/15. Hand delivered to Bumpys office by John Shaft."
Text in: June 6, 2028 Check #14732 to Bumpy Trucking and Garbage Collection, annotation: services rendered 4/15. Hand delivered to Bumpys office by John Shaft.
Numeric representation:
4421547151858877364227208750908738620733607601859739372166778341524785742550807474034193837670295505831104819159095432207230080492609913152702863057013041792095154406751851568447861921620672338622912730786271117430703079263978347455540995607801300412823380959215323585415884495086046257201591510925531421584710679242
  Number of decimal digits is 316

You generate the random number with an easy to remember seed.

./RanDigits 316 666
5798918755684995343251197670131001943162624124093318531648706816441157354800610352640709079604798207487808913098652027938939712643725510356795951564361605570274557512088697237334104769650778678733036235363036996302668746003870517141111429458139706924524345707565187664619567887879372100721877829690455856584312749394

Now combine the two numbers.

./CombineNum 4421547151858877364227208750908738620733607601859739372166778341524785742550807474034193837670295505831104819159095432207230080492609913152702863057013041792095154406751851568447861921620672338622912730786271117430703079263978347455540995607801300412823380959215323585415884495086046257201591510925531421584710679242 5798918755684995343251197670131001943162624124093318531648706816441157354800610352640709079604798207487808913098652027938939712643725510356795951564361605570274557512088697237334104769650778678733036235363036996302668746003870517141111429458139706924524345707565187664619567887879372100721877829690455856584312749394
1831762323861249837930829237514969721520249540032510806699111531783846695230315687220506944978717078755301855511508198800261931258638137644570018463913814557736900165010385428652465858341227473331596263997699498135281951732408914503470680909292997098709070729059822506981013808638028803407447840297099417096384169880

With a nice obscure numeric message you can create a revised chain from the
template chain. We paste the number into a file foo.txt which we will
delete, and perform the substitution into a new chain.

./NumToChain ProctoDemo.template.chain.txt foo.txt  > Msg1.chain.txt

From which we create a substitution text.

./Totg ProctoDemo.saurus.txt ProctoDemo.template.txt Msg1.chain.txt > Msg1.substitution.txt

The day is over and we delete everything but the substitution text, which
stays laying around with the template looking like we were having a really
bad day typing or something, but nobody’s really paying attention.

This goes on for a while. You accumulate more evidence, then the day comes
when you decide to unpack it all and reveal everything to uh… somebody.

You go back to the substitution text and generate a new chain.

./Totg_dbg -rw ProctoDemo.saurus.txt Msg1.substitution.txt > Msg1.substitution.chain.txt

Translate the chain to a numeric value.

./ChainToNum Msg1.substitution.chain.txt
1831762323861249837930829237514969721520249540032510806699111531783846695230315687220506944978717078755301855511508198800261931258638137644570018463913814557736900165010385428652465858341227473331596263997699498135281951732408914503470680909292997098709070729059822506981013808638028803407447840297099417096384169880


The reader can see already that it is the same number that was put in, but
it has been a while, so you don’t get excited yet. The number was combined
with a psuedo-random generated with 323 digits and seed 666.

./RanDigits 316 666
5798918755684995343251197670131001943162624124093318531648706816441157354800610352640709079604798207487808913098652027938939712643725510356795951564361605570274557512088697237334104769650778678733036235363036996302668746003870517141111429458139706924524345707565187664619567887879372100721877829690455856584312749394

Recombine the numbers:

./CombineNum 1831762323861249837930829237514969721520249540032510806699111531783846695230315687220506944978717078755301855511508198800261931258638137644570018463913814557736900165010385428652465858341227473331596263997699498135281951732408914503470680909292997098709070729059822506981013808638028803407447840297099417096384169880 5798918755684995343251197670131001943162624124093318531648706816441157354800610352640709079604798207487808913098652027938939712643725510356795951564361605570274557512088697237334104769650778678733036235363036996302668746003870517141111429458139706924524345707565187664619567887879372100721877829690455856584312749394
4421547151858877364227208750908738620733607601859739372166778341524785742550807474034193837670295505831104819159095432207230080492609913152702863057013041792095154406751851568447861921620672338622912730786271117430703079263978347455540995607801300412823380959215323585415884495086046257201591510925531421584710679242


Then convert back to text.

./NumToText 4421547151858877364227208750908738620733607601859739372166778341524785742550807474034193837670295505831104819159095432207230080492609913152702863057013041792095154406751851568447861921620672338622912730786271117430703079263978347455540995607801300412823380959215323585415884495086046257201591510925531421584710679242
June 6, 2028 Check #14732 to Bumpy Trucking and Garbage Collection, annotation: services rendered 4/15. Hand delivered to Bumpys office by John Shaft.

Which is the desired recovery. Q.E.D.

### Summary

So, the general response: “Are you kidding? Nobody’s going to go through all
that. Something like that is a tough sell even to sys admins and and skilled
CLI users.” True. It’s definitely not a product. The reason I strung
everything out this way is to insure that everything is exposed. This
collection of code can be re-formed into a GUI-based system in many ways.
Clearly, if you want others to use it you would need to spend more time on
specific content of the saurus files and similar issues. This demo is
marginally usable. My intention is to create executable code than can be
read and hacked. I may do a more polished job in another repo.

Another aspect of this that I’m hoping to communicate is that this
marginally readable text substitution is a huge simplification of the
general algorithm. Making all the associated data readable text has the
advantage that every item can be easily traced through the system with grep
and the like, and examined with any text editor. An image-based system with
each data object being a binary file of at least a few megabytes is a much
more practical candidate for a usable, practical system, but – crucially –
would you, dear reader, have made it this far through that much binary data
unless I had written this repo as a much longer and more detailed read?

Anyway, I hope you’ve enjoyed this and been stimulated. If you’ve followed
along with the text, I’m thinking you could write the image version yourself
right now. Happy coding. May the farce be with you.




