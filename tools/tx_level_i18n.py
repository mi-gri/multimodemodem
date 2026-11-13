LANGS=('en','it','fr','de','no','cs')
VALUES={
 'tx_level_tooltip':(
  'TX level: digital gain of the transmitted audio, 0.0 dB = full scale. Sets the drive and so the RF power. Remembered for each band and mode; during Tune it sets the Tune level of the band.',
  'Livello TX: guadagno digitale dell’audio trasmesso, 0,0 dB = fondo scala. Regola il pilotaggio e quindi la potenza RF. Memorizzato per banda e modo; durante Sintonia regola il livello di sintonia della banda.',
  'Niveau TX : gain numérique de l’audio émis, 0,0 dB = pleine échelle. Règle l’excitation et donc la puissance HF. Mémorisé par bande et par mode ; pendant l’accord, il règle le niveau d’accord de la bande.',
  'TX-Pegel: digitale Verstärkung des Sende-Audios, 0,0 dB = Vollaussteuerung. Bestimmt die Ansteuerung und damit die HF-Leistung. Wird je Band und Modus gemerkt; während Abstimmen stellt er den Abstimmpegel des Bandes ein.',
  'TX-nivå: digital forsterkning av sendt lyd, 0,0 dB = full skala. Bestemmer utstyringen og dermed RF-effekten. Huskes for hvert bånd og modus; under avstemming stiller den båndets avstemmingsnivå.',
  'Úroveň TX: digitální zesílení vysílaného zvuku, 0,0 dB = plné vybuzení. Určuje buzení a tím výkon VF. Pamatuje se pro každé pásmo a druh provozu; během ladění nastavuje úroveň ladění pásma.'),
 'tune_level_prefix':('Tune','Sintonia','Tune','Tune','Tune','Ladění'),
}
KEYS={key:dict(zip(LANGS,values)) for key,values in VALUES.items()}
