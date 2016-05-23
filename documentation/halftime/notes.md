# Halftime presentation
Martin & Daniel
### Important stuff to take up
* Problem
* Types of machine learning
* Evolutionary/Reinforcement learning
* Simulator
* XOR experiment
* Fixed & Evolving network
* Input/output problems
* Statefull algorithms
* Continous reality


#Introduction
Hej svejs!

#Problembeskrivning
Raceingförare
Linjer
    Väder
    Strategier
    



#Limitations
Begränsad realism
hot lap, ingen head to head racing


#Method 

## Simulation

För att ha en miljö för vår AI att lära sig i och utvärderas i har vi byggt en enkel simulator. 

Skulle kunna använda oss av färdiga spelmotorer, som har väldigt bra fysiksimuleringarm, bilsimuleringar.

Har dock valt att göra en egen för att kunna ha full kontroll på vad som ingår i simuleringen och hur den körs etc. Skall kunna köra med & utan visualisering, eventuellt köra många simuleringar parallellt. 

## Utvärdering
Lite krångligt att utvärdera hur bra olika AI-instanser presterar. Enkelt att jämföra dem sinsemellan. Går inte direkt att jämföra med verkligheten pga. den begränsade realismen i vår simulator. 

Också svårt att jämföra med racing-teori då det inte finns ETT rätt sätt, det kan skilja sig mellan bil/förare/sitatuion osv. Dock kan vi se ifall vårt program kan lära sig vissa koncept, som positionering. 

## Maskininlärning

Fält inom artificiell intelligens. AIn lär sig själv istället för att man designar den i minsta detalj. 

Vi behöver hitta en kunskapsmodell och algoritm som kan lära sig att styra bilen. 

Olika kunskapsmodeller, neurala nätverk, Support Vector Machines, Beslutsträd. 

Många olika typer av maskininlärning.

* Supervised learning 

Man ger träningsalgoritmen en stor mängd träningsdata för att lära den ett mönster eller regel mellan in- och utdata. Används ofta för att klassifiera saker, t.ex bildigenkänning. T.ex. kan man använda detta för att träna en dator att kunna identiera att en bild är en bild på en katt. 

Kan vara problematiskt för oss eftersom vi inte har tillgång till någon exempeldata för hur man skall styra en formel1-bil i olika situationer. Underlättar också ifall man har diskret utdata. 

* Unsupervised Learning
Lära AIn att hitta mönster i indata, används inom data mining, clustering, t.ex. para ihop olika typer av bilder. 
Ifall supervised learning kan lära en dator att säga att en bild av en katt är en bild av en katt kan unsupervised lära sig att ifall den får en bild av en katt så hör den bilden ihop med alla de andra kattbilderna, utan att veta vad det är på bilden. Inte riktigt relevant för oss. 

En tredje typ av maskininlärning som är mer lovande för oss är reinforcement learning. 
Detta går ut på att en AI interagerar med en dynamisk värld där den utvärderas beroende på hur bra den presterar, bra val leder till en ökad prestation och dåliga till en sämre. I vårt fall kan vi mäta prestationen i hur långt bilen kan ta sig utan att köra av vägen samt hur snabbt den kör. 

Träna AIn beroende på prestation, förstärk bra beteende. 

En variant är neat, Neuroevolution through augmenting topologies.
Kenneth Stanley & Risto Miikkulainen, University of Texas.
Neurala nätverk, evolution, både vikter & kanter. 
Survival of the fittest, Bra nätverk överlever, dåliga dör.

Man testar en stor population av nätverk, dödar av den sämre hälften och odlar fram en ny generation genom att kombinera och mutera de kvarvarande individerna. 

#Results

## Current progress

Vi har implementerat vår simulator. Enkel fysik & rendering. 
Implementerat neurala nätvärk med en fast struktur, enkel optimeringsmetod för att hitta vikter.
En implementation av NEAT. 



## Demo
Bilen klarar av att hålla sig på mitten av vägen. Avstånd och vinkel till mittlinjen som indata, utdata är bara styrning.

# Forsättning

Forstätta utveckla simulatorn och träningsalgoritmerna.

Hitta bra representationer av in & utdata. Vilken information är relevant för att kunna lösa problemet. Vilka beslut kommer datorn klara av att ta. 

#Questions?