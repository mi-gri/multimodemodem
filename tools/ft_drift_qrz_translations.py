# Reviewed UI phrases for the optional FT drift recovery and persistent QRZ account.
PHRASES = [
    ("Username", "Nome utente", "Nom d’utilisateur", "Benutzername", "Brukernavn", "Uživatelské jméno"),
    ("Password", "Password", "Mot de passe", "Passwort", "Passord", "Heslo"),
    ("RX drift compensation", "Compensazione drift RX", "Compensation de dérive RX", "RX-Driftkompensation", "RX-driftkompensasjon", "Kompenzace driftu RX"),
    ("Recover drifting signals near the RX marker (experimental)",
     "Recupera segnali con drift vicino al marcatore RX (sperimentale)",
     "Récupérer les signaux dérivants près du marqueur RX (expérimental)",
     "Driftende Signale nahe dem RX-Marker wiederherstellen (experimentell)",
     "Gjenfinn drivende signaler nær RX-markøren (eksperimentelt)",
     "Obnovovat signály s driftem poblíž RX markeru (experimentální)"),
    ("Maximum drift (±)", "Drift massimo (±)", "Dérive maximale (±)", "Maximale Drift (±)", "Maksimal drift (±)", "Maximální drift (±)"),
    ("The account is saved in the local profile and reused after restart. QRZ XML access is required for complete station data.",
     "L’account viene salvato nel profilo locale e riutilizzato al riavvio. Per i dati completi della stazione serve l’accesso XML QRZ.",
     "Le compte est enregistré dans le profil local et réutilisé au redémarrage. Les données complètes nécessitent un accès XML QRZ.",
     "Das Konto wird im lokalen Profil gespeichert und nach dem Neustart wiederverwendet. Vollständige Stationsdaten erfordern QRZ-XML-Zugang.",
     "Kontoen lagres i den lokale profilen og brukes etter omstart. Fullstendige stasjonsdata krever QRZ XML-tilgang.",
     "Účet se ukládá do místního profilu a použije se po restartu. Úplné údaje o stanici vyžadují přístup QRZ XML."),
    ("Select the signal with the RX marker. This option helps decode it when its frequency changes during reception. You control the transmit frequency. Results appear in the runtime log.",
     "Posiziona il marcatore RX sul segnale da seguire. Questa opzione aiuta a decodificarlo se la frequenza cambia durante la ricezione. La frequenza TX resta quella impostata. I risultati compaiono nel registro.",
     "Placez le marqueur RX sur le signal à suivre. Cette option aide à le décoder si sa fréquence change pendant la réception. Vous réglez la fréquence d’émission. Les résultats apparaissent dans le journal.",
     "Setzen Sie den RX-Marker auf das gewünschte Signal. Diese Option hilft beim Dekodieren, wenn sich seine Frequenz während des Empfangs ändert. Die Sendefrequenz stellen Sie selbst ein. Ergebnisse stehen im Laufzeitprotokoll.",
     "Plasser RX-markøren på signalet du vil følge. Dette alternativet hjelper med dekoding når frekvensen endres under mottak. Du velger sendefrekvensen selv. Resultatene vises i loggen.",
     "Umístěte RX marker na sledovaný signál. Tato volba pomáhá s dekódováním, pokud se jeho frekvence během příjmu mění. Vysílací frekvenci nastavujete sami. Výsledky se zobrazují v protokolu."),
]
QRZ_SETTINGS = {
    'it': 'Configura l’account QRZ in Impostazioni → QRZ.com. I dati completi richiedono l’accesso XML.',
    'fr': 'Configurez le compte QRZ dans Paramètres → QRZ.com. Les données complètes nécessitent un accès XML.',
    'de': 'QRZ-Konto unter Einstellungen → QRZ.com konfigurieren. Vollständige Daten erfordern XML-Zugang.',
    'no': 'Konfigurer QRZ-kontoen i Innstillinger → QRZ.com. Fullstendige data krever XML-tilgang.',
    'cs': 'Nastavte účet QRZ v Nastavení → QRZ.com. Úplné údaje vyžadují přístup XML.',
}

PHRASES.append(('Active only at 144 MHz and above, up to ±20 Hz/s. Uses CAT frequency, or the selected band without CAT. Keep the entire drifting signal inside the receiver passband.', 'Attivo solo da 144 MHz in su, fino a ±20 Hz/s. Usa la frequenza CAT oppure la banda selezionata senza CAT. Mantieni tutto il segnale in deriva dentro la banda passante del ricevitore.', 'Actif uniquement à partir de 144 MHz, jusqu’à ±20 Hz/s. Utilise la fréquence CAT ou la bande sélectionnée sans CAT. Gardez tout le signal dérivant dans la bande passante du récepteur.', 'Nur ab 144 MHz aktiv, bis ±20 Hz/s. Verwendet die CAT-Frequenz oder ohne CAT das gewählte Band. Das gesamte driftende Signal muss im Durchlassbereich des Empfängers bleiben.', 'Aktiv kun fra 144 MHz, opptil ±20 Hz/s. Bruker CAT-frekvensen eller valgt bånd uten CAT. Hold hele signalet innenfor mottakerens passbånd.', 'Aktivní pouze od 144 MHz, až ±20 Hz/s. Používá frekvenci CAT nebo zvolené pásmo bez CAT. Celý driftující signál musí zůstat v propustném pásmu přijímače.'))

PHRASES.extend([('Try GPU drift sync (experimental, automatic CPU fallback)', 'Prova GPU per il drift (sperimentale, ritorno automatico alla CPU)', 'Essayer le GPU pour la dérive (expérimental, repli CPU automatique)', 'GPU für Drift testen (experimentell, automatischer CPU-Rückfall)', 'Prøv GPU for drift (eksperimentelt, automatisk CPU-reserve)', 'Vyzkoušet GPU pro drift (experimentální, automatický návrat na CPU)'), ('Requires an OpenCL GPU with double precision. The first batch checks CPU parity and total time; slower or failed devices keep the CPU path. Applies to drift recovery from 144 MHz.', 'Richiede una GPU OpenCL con doppia precisione. Il primo blocco confronta risultati e tempo totale con la CPU; se la GPU è più lenta o dà errori, resta attiva la CPU. Vale per il recupero drift da 144 MHz.', 'Nécessite un GPU OpenCL en double précision. Le premier lot compare les résultats et le temps total au CPU ; si le GPU est plus lent ou échoue, le CPU reste actif. Pour la dérive à partir de 144 MHz.', 'Erfordert eine OpenCL-GPU mit doppelter Genauigkeit. Der erste Stapel vergleicht Ergebnisse und Gesamtzeit mit der CPU; bei langsamerer GPU oder Fehlern bleibt die CPU aktiv. Für Drift ab 144 MHz.', 'Krever OpenCL-GPU med dobbel presisjon. Første gruppe sammenligner resultater og totaltid med CPU; ved feil eller tregere GPU brukes CPU. Gjelder drift fra 144 MHz.', 'Vyžaduje OpenCL GPU s dvojitou přesností. První dávka porovná výsledky a celkový čas s CPU; při chybě nebo pomalejší GPU zůstává CPU. Pro drift od 144 MHz.')])

PHRASES.extend([
 ("Try GPU FT8/FT4 decoding (experimental, all bands)", "Prova decodifica FT8/FT4 con GPU (sperimentale, tutte le bande)", "Essayer le décodage FT8/FT4 sur GPU (expérimental, toutes bandes)", "FT8/FT4-Dekodierung mit GPU testen (experimentell, alle Bänder)", "Prøv FT8/FT4-dekoding med GPU (eksperimentelt, alle bånd)", "Vyzkoušet dekódování FT8/FT4 na GPU (experimentální, všechna pásma)"),
 ("Offloads symbol tone energies to OpenCL FP64. Checks CPU parity and total time, with automatic CPU fallback. Independent of drift compensation.", "Calcola le energie dei toni dei simboli con OpenCL FP64. Confronta risultati e tempo totale con la CPU e torna automaticamente alla CPU se necessario. Indipendente dalla compensazione drift.", "Calcule les énergies des tons avec OpenCL FP64. Compare les résultats et le temps total au CPU, avec repli automatique. Indépendant de la compensation de dérive.", "Berechnet Symboltonenergien mit OpenCL FP64. Vergleicht Ergebnisse und Gesamtzeit mit der CPU, mit automatischem Rückfall. Unabhängig von der Driftkompensation.", "Beregner symboltonenes energi med OpenCL FP64. Sammenligner resultater og totaltid med CPU, med automatisk CPU-reserve. Uavhengig av driftkompensasjon.", "Počítá energie tónů symbolů pomocí OpenCL FP64. Porovnává výsledky a celkový čas s CPU, s automatickým návratem. Nezávislé na kompenzaci driftu.")
])

# Late decodes after the reply deadline (FT8/FT4 decoder page).
PHRASES.extend([
 ("FT8 / FT4 late decodes", "Decodifiche tardive FT8 / FT4", "Décodages tardifs FT8 / FT4", "Späte Dekodierungen FT8 / FT4", "Sene dekodinger FT8 / FT4", "Pozdní dekódování FT8 / FT4"),
 ("Keep searching after the reply deadline and show late decodes",
  "Continua a cercare dopo il termine per la risposta e mostra le decodifiche tardive",
  "Continuer la recherche après le délai de réponse et afficher les décodages tardifs",
  "Nach der Antwortfrist weitersuchen und späte Dekodierungen anzeigen",
  "Søk videre etter svarfristen og vis sene dekodinger",
  "Hledat i po lhůtě pro odpověď a zobrazit pozdní dekódování"),
 ("The deep search goes on for up to 9 s (FT4 4.5 s) into the next period. Late decodes are only shown: they never answer, call or start TX. Uses about two CPU cores during that time.",
  "La ricerca approfondita continua fino a 9 s (FT4 4,5 s) nel periodo successivo. Le decodifiche tardive sono solo mostrate: non rispondono, non chiamano e non avviano la TX. In quel tempo usa circa due core della CPU.",
  "La recherche approfondie continue jusqu’à 9 s (FT4 4,5 s) dans la période suivante. Les décodages tardifs sont seulement affichés : ils ne répondent pas, n’appellent pas et ne lancent pas l’émission. Utilise environ deux cœurs CPU pendant ce temps.",
  "Die gründliche Suche läuft bis zu 9 s (FT4 4,5 s) in die nächste Periode weiter. Späte Dekodierungen werden nur angezeigt: Sie beantworten, rufen und senden nie. Belegt währenddessen etwa zwei CPU-Kerne.",
  "Det grundige søket fortsetter opptil 9 s (FT4 4,5 s) inn i neste periode. Sene dekodinger vises bare: de svarer, kaller og sender aldri. Bruker omtrent to CPU-kjerner i den tiden.",
  "Důkladné hledání pokračuje až 9 s (FT4 4,5 s) do další periody. Pozdní dekódování se jen zobrazí: nikdy neodpovídá, nevolá ani nespouští vysílání. Během té doby využívá asi dvě jádra CPU."),
])

# Auto mode switch and its legal/safety notice (Settings -> FT decoder).
PHRASES.extend([
 ("Auto mode", "Modalità automatica", "Mode automatique", "Auto-Modus", "Automatisk modus", "Automatický režim"),
 ("Enable Auto mode (FT8/FT4 Auto QSO, JS8 automatic operation)", "Attiva la modalità automatica (Auto QSO FT8/FT4, funzionamento automatico JS8)", "Activer le mode automatique (Auto QSO FT8/FT4, fonctionnement automatique JS8)", "Auto-Modus einschalten (FT8/FT4 Auto QSO, JS8-Automatik)", "Slå på automatisk modus (FT8/FT4 Auto QSO, automatisk JS8-drift)", "Zapnout automatický režim (Auto QSO FT8/FT4, automatický provoz JS8)"),
 ("Lets Auto QSO answer CQ calls and complete QSOs, and JS8 answer queries, relay messages and send heartbeats or CQ by timer. Switching it on shows a legal and safety notice that must be confirmed.",
  "Permette ad Auto QSO di rispondere ai CQ e completare i QSO, e a JS8 di rispondere alle richieste, inoltrare messaggi e inviare heartbeat o CQ a intervalli. All’attivazione compare un avviso legale e di sicurezza da confermare.",
  "Permet à Auto QSO de répondre aux CQ et de mener les QSO, et à JS8 de répondre aux requêtes, relayer des messages et envoyer heartbeats ou CQ à intervalles. L’activation affiche un avertissement juridique et de sécurité à confirmer.",
  "Erlaubt Auto QSO, CQ-Rufe zu beantworten und QSOs abzuwickeln, und JS8, Anfragen zu beantworten, Nachrichten weiterzuleiten sowie Heartbeats oder CQ per Zeitplan zu senden. Beim Einschalten erscheint ein Rechts- und Sicherheitshinweis, der bestätigt werden muss.",
  "Lar Auto QSO svare på CQ og fullføre QSO-er, og JS8 svare på forespørsler, videresende meldinger og sende heartbeat eller CQ etter tidsplan. Når den slås på, vises en juridisk og sikkerhetsmessig merknad som må bekreftes.",
  "Umožní Auto QSO odpovídat na CQ a dokončovat spojení a JS8 odpovídat na dotazy, předávat zprávy a odesílat heartbeat nebo CQ podle časovače. Při zapnutí se zobrazí právní a bezpečnostní upozornění, které je nutné potvrdit."),
 ("Enable Auto mode", "Attiva la modalità automatica", "Activer le mode automatique", "Auto-Modus einschalten", "Slå på automatisk modus", "Zapnout automatický režim"),
 ("Auto mode answers CQ calls and completes QSOs (FT8/FT4), answers queries, relays messages and sends heartbeats or CQ by timer (JS8) without your intervention.",
  "La modalità automatica risponde ai CQ e completa i QSO (FT8/FT4), risponde alle richieste, inoltra messaggi e invia heartbeat o CQ a intervalli (JS8) senza il tuo intervento.",
  "Le mode automatique répond aux CQ et mène les QSO (FT8/FT4), répond aux requêtes, relaie des messages et envoie heartbeats ou CQ à intervalles (JS8) sans votre intervention.",
  "Der Auto-Modus beantwortet CQ-Rufe und wickelt QSOs ab (FT8/FT4), beantwortet Anfragen, leitet Nachrichten weiter und sendet Heartbeats oder CQ per Zeitplan (JS8) – ohne dein Zutun.",
  "Automatisk modus svarer på CQ og fullfører QSO-er (FT8/FT4), svarer på forespørsler, videresender meldinger og sender heartbeat eller CQ etter tidsplan (JS8) uten at du griper inn.",
  "Automatický režim odpovídá na CQ a dokončuje spojení (FT8/FT4), odpovídá na dotazy, předává zprávy a odesílá heartbeat nebo CQ podle časovače (JS8) bez vašeho zásahu."),
 ("Automatic operation may not be permitted in every country. Check the rules of your licence.",
  "Il funzionamento automatico potrebbe non essere consentito in tutti i paesi. Verifica le norme della tua licenza.",
  "Le fonctionnement automatique n’est peut-être pas autorisé dans tous les pays. Vérifiez les règles de votre licence.",
  "Automatischer Betrieb ist eventuell nicht in allen Ländern erlaubt. Prüfe die Bestimmungen deiner Zulassung.",
  "Automatisk drift er kanskje ikke tillatt i alle land. Sjekk reglene for lisensen din.",
  "Automatický provoz nemusí být povolen ve všech zemích. Ověřte pravidla svého oprávnění."),
 ("In contests, automatic operation can lead to disqualification. Check the contest rules.",
  "Nei contest il funzionamento automatico può portare alla squalifica. Verifica il regolamento del contest.",
  "En concours, le fonctionnement automatique peut entraîner une disqualification. Vérifiez le règlement du concours.",
  "Bei Contesten kann automatischer Betrieb zur Disqualifikation führen. Prüfe die Contest-Regeln.",
  "I konkurranser kan automatisk drift føre til diskvalifisering. Sjekk konkurransereglene.",
  "V závodech může automatický provoz vést k diskvalifikaci. Ověřte pravidla závodu."),
 ("Do not leave the station unattended for long periods. You remain responsible for every transmission.",
  "Non lasciare la stazione incustodita a lungo. Resti responsabile di ogni trasmissione.",
  "Ne laissez pas la station sans surveillance pendant longtemps. Vous restez responsable de chaque émission.",
  "Lass die Station nicht längere Zeit unbeaufsichtigt. Du bleibst für jede Aussendung verantwortlich.",
  "Ikke la stasjonen stå uten tilsyn i lengre tid. Du er fortsatt ansvarlig for hver sending.",
  "Nenechávejte stanici delší dobu bez dozoru. Za každé vysílání odpovídáte vy."),
 ("You alone are responsible for any damage that may result.",
  "Sei l’unico responsabile di eventuali danni.",
  "Vous êtes seul responsable des dommages éventuels.",
  "Für eventuell entstehende Schäden bist allein du verantwortlich.",
  "Du alene er ansvarlig for eventuelle skader.",
  "Za případné škody odpovídáte výhradně vy."),
 ("I have read and understood your notice and want to continue",
  "Ho letto e compreso l’avviso e desidero continuare",
  "J’ai lu et compris l’avertissement et je souhaite continuer",
  "Ich habe deinen Hinweis gelesen und verstanden und möchte fortfahren",
  "Jeg har lest og forstått merknaden og vil fortsette",
  "Přečetl(a) jsem upozornění, rozumím mu a chci pokračovat"),
])
