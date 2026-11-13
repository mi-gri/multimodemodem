# SPDX-License-Identifier: GPL-3.0-or-later
"""German translations for visible texts that were still English.

Keyed by the English source text, so a translation applies to every key
whose English text is the same (uiText keys, generated text.<source> keys of
labels, tooltips, What's This help and dialog-local L() calls).  Found with
`MultiModeModem --i18n-report` in German.  update_ui_translations.py also
registers text.<source> keys for these sources when they occur in the code
(help texts set through setHelpText() are not harvested otherwise).
"""

DE = {
    # Main window, mode pages: help and tooltips
    "Abort the active image transmission and release RTS PTT.":
        "Bricht die laufende Bildaussendung ab und gibt die RTS-PTT frei.",
    "Audio input, audio output and PTT serial port are configured from Settings → Audio / PTT settings.":
        "Audioeingang, Audioausgang und serielle PTT-Schnittstelle werden unter Einstellungen → Audio / PTT eingestellt.",
    "Audio mark tone in Hz. High-tone AFSK commonly uses 2125 Hz; low-tone AFSK often uses 1275 Hz. Waterfall click tuning sets this marker for the current session.":
        "Audio-Mark-Ton in Hz. AFSK mit hohen Tönen nutzt meist 2125 Hz, mit tiefen Tönen oft 1275 Hz. Ein Klick in den Wasserfall setzt diese Markierung für die laufende Sitzung.",
    "Automatically save a completed WEFAX frame as PNG using WeatherFAX_YYYYMMDD_HHMMSS.png in the selected folder.":
        "Speichert ein fertiges WEFAX-Bild automatisch als PNG (WeatherFAX_JJJJMMTT_HHMMSS.png) im gewählten Ordner.",
    "CRT-style RTTY Mark/Space tuning scope. It shows only the live tuning trace and the logical Mark/Space labels; decoded text belongs to the terminal or optional waterfall overlay.":
        "RTTY-Abstimmanzeige für Mark/Space im Stil einer Bildröhre. Sie zeigt nur die laufende Abstimmspur und die logischen Mark/Space-Beschriftungen; dekodierter Text erscheint im Terminal oder optional im Wasserfall.",
    "Center frequency of the MFSK tone set. Click the waterfall to retune it directly.":
        "Mittenfrequenz des MFSK-Tonsatzes. Ein Klick in den Wasserfall stimmt sie direkt um.",
    "Clear only RX A text and reset RX A acquisition; RX B keeps running.":
        "Löscht nur den Text von RX A und setzt dessen Erfassung zurück; RX B läuft weiter.",
    "Clear only RX B text and reset RX B acquisition; RX A keeps running.":
        "Löscht nur den Text von RX B und setzt dessen Erfassung zurück; RX A läuft weiter.",
    "Clear the Hellschreiber transmission text.": "Löscht den Hellschreiber-Sendetext.",
    "Clear the Hellschreiber virtual receive paper.": "Löscht das virtuelle Hellschreiber-Empfangspapier.",
    "Clear the MFSK transmission text.": "Löscht den MFSK-Sendetext.",
    "Clear the PSK transmission text.": "Löscht den PSK-Sendetext.",
    "Clear the RTTY transmission text.": "Löscht den RTTY-Sendetext.",
    "Clear the current MeteoFax image and restart the decoder start/stop state machine.":
        "Löscht das aktuelle Wetterfax-Bild und startet die Start/Stopp-Erkennung des Decoders neu.",
    "Clear the received MFSK text window.": "Löscht das MFSK-Empfangsfenster.",
    "Clear the received PSK text window.": "Löscht das PSK-Empfangsfenster.",
    "Clear the received RTTY text window.": "Löscht das RTTY-Empfangsfenster.",
    "Compatibility PTT serial selector. Prefer Settings > Audio / PTT settings or CAT / Rig for normal configuration.":
        "PTT-Schnittstellenauswahl aus Kompatibilitätsgründen. Eingestellt wird normalerweise unter Einstellungen > Audio / PTT oder CAT / Funkgerät.",
    "Compatibility device selector. The real audio/PTT setup is in Settings > Audio / PTT settings; this field mirrors saved devices for older layouts.":
        "Geräteauswahl aus Kompatibilitätsgründen. Audio und PTT werden unter Einstellungen > Audio / PTT eingestellt; dieses Feld zeigt nur die gespeicherten Geräte für ältere Ansichten.",
    "Compatibility stop control kept hidden; the Status RX button now toggles live RX.":
        "Verborgener Stopp-Knopf aus Kompatibilitätsgründen; der RX-Knopf im Status schaltet den Live-Empfang.",
    "Complete and optionally auto-save the frame when the decoder sees an APT stop/end tone or when the useful WEFAX signal disappears for the timeout.":
        "Schließt das Bild ab und speichert es auf Wunsch, wenn der Decoder einen APT-Stopp-/Endton erkennt oder das WEFAX-Signal für die eingestellte Zeit ausbleibt.",
    "Configure CAT frequency polling, PTT, Hamlib radios and Ham Radio Deluxe TCP control.":
        "CAT-Frequenzabfrage, PTT, Hamlib-Funkgeräte und Steuerung über Ham Radio Deluxe (TCP) einstellen.",
    "Current decoder state and diagnostic text from the selected modem.":
        "Aktueller Decoderzustand und Diagnosetext des gewählten Modems.",
    "Current input audio level. Keep it safely below clipping while still high enough for reliable decoding.":
        "Aktueller Audio-Eingangspegel. Sicher unter der Übersteuerung halten, aber hoch genug für zuverlässiges Dekodieren.",
    "Decode a WAV recording directly through the MeteoFax DSP path without playing it to speakers. WAV analysis uses manual/free-running timing from the first sample, then locks the first stable WEFAX sync stripe before showing/saving rows.":
        "Dekodiert eine WAV-Aufnahme direkt über die Wetterfax-Signalverarbeitung, ohne sie abzuspielen. Die Analyse läuft ab dem ersten Sample frei und rastet auf den ersten stabilen WEFAX-Synchronstreifen ein, bevor Zeilen angezeigt oder gespeichert werden.",
    "Decoded Morse text from the exact blue RX B receiver.": "Dekodierter Morsetext des blauen Empfängers RX B.",
    "Decoded Morse text from the exact green RX A receiver.": "Dekodierter Morsetext des grünen Empfängers RX A.",
    "Decoded PSK text using the Varicode pipeline.": "Dekodierter PSK-Text (Varicode).",
    "Decoded RTTY text. Uses ITA2/Baudot letters/figures shift.": "Dekodierter RTTY-Text (ITA2/Baudot mit Buchstaben-/Ziffernumschaltung).",
    "Dominant audio frequency estimated by the common DSP engine and shown on the waterfall.":
        "Stärkste Audiofrequenz, von der Signalverarbeitung geschätzt und im Wasserfall angezeigt.",
    "Edit the label and transmitted text of the active RTTY macro bank.":
        "Beschriftung und Sendetext der aktiven RTTY-Makrobank bearbeiten.",
    "Enable narrow AFC around the Hellschreiber center marker. The paper follows the strongest local keyed-tone energy peak.":
        "Schmale AFC um die Hellschreiber-Mittenmarkierung. Das Papier folgt dem stärksten getasteten Ton in der Nähe.",
    "Enable narrow AFC around the selected PSK tone. Useful for small audio/radio drift; disable for exact loopback tests.":
        "Schmale AFC um den gewählten PSK-Ton. Hilfreich bei leichter Drift von Audio oder Funkgerät; für exakte Loopback-Tests ausschalten.",
    "Enable slow AFC on the MFSK tone bank to follow small audio-frequency offsets.":
        "Langsame AFC für den MFSK-Tonsatz, um kleinen Frequenzablagen zu folgen.",
    "End-of-signal timeout in seconds. Increase it for noisy HF channels; decrease it for faster unattended saving after the control/end tone or signal loss.":
        "Wartezeit in Sekunden bis zum Signalende. Bei verrauschter Kurzwelle erhöhen; verringern, damit nach Endton oder Signalverlust schneller gespeichert wird.",
    "Folder used for automatic PNG saving. The folder is created automatically if it does not exist.":
        "Ordner für das automatische Speichern als PNG. Er wird angelegt, falls er noch nicht existiert.",
    "Frequency shift between mark and space tones. Common amateur narrow shift is 170 Hz; utility modes often use 425 or 850 Hz.":
        "Abstand zwischen Mark- und Space-Ton. Im Amateurfunk sind 170 Hz üblich, kommerzielle Dienste nutzen oft 425 oder 850 Hz.",
    "HF WEFAX is usually 120 LPM / IOC 576 with black near 1500 Hz and white near 2300 Hz. Station presets provide practical defaults; auto-save writes WeatherFAX_date_time.png when a control/end tone or end-of-signal timeout completes the frame.":
        "Kurzwellen-WEFAX läuft meist mit 120 LPM / IOC 576, Schwarz bei etwa 1500 Hz und Weiß bei etwa 2300 Hz. Die Stationsvorgaben liefern passende Werte; das automatische Speichern schreibt WeatherFAX_Datum_Uhrzeit.png, sobald ein Endton oder das Signalende das Bild abschließt.",
    "Hellschreiber center carrier tone in Hz. Click the waterfall to retune this session-only marker directly.":
        "Hellschreiber-Mittenton in Hz. Ein Klick in den Wasserfall setzt diese Markierung direkt (nur für diese Sitzung).",
    "Image transmission progress. The shaded area on the preview shows pixels/lines already transmitted.":
        "Fortschritt der Bildaussendung. Der schattierte Bereich in der Vorschau zeigt die schon gesendeten Zeilen.",
    "Image zoom controls. Mouse wheel zooms over the image; drag pans when zoomed in.":
        "Bildvergrößerung. Das Mausrad zoomt im Bild; vergrößert lässt es sich mit der Maus verschieben.",
    "Internal safety maximum only. The received image grows line by line and no artificial image buffer is shown.":
        "Nur eine interne Obergrenze. Das empfangene Bild wächst Zeile für Zeile; es wird kein künstlicher Bildpuffer angezeigt.",
    "Invert decoded/transmitted bits for diagnostics. Normally leave OFF for standard PSK.":
        "Kehrt empfangene und gesendete Bits zur Diagnose um. Für normales PSK ausgeschaltet lassen.",
    "Invert mark/space logic. Enable when decoded text is garbage but the signal is otherwise strong and centered.":
        "Vertauscht Mark und Space. Einschalten, wenn bei starkem, mittig liegendem Signal nur Zeichensalat kommt.",
    "Load a still image for TX. Supported formats depend on Qt image plugins, typically PNG, JPEG/JPG and BMP.":
        "Lädt ein Bild zum Senden. Die Formate hängen von den Qt-Bildmodulen ab, meist PNG, JPEG/JPG und BMP.",
    "Load plain text for Hellschreiber transmission.": "Lädt einfachen Text zum Senden in Hellschreiber.",
    "Load plain text for MFSK transmission.": "Lädt einfachen Text zum Senden in MFSK.",
    "Load plain text for PSK transmission.": "Lädt einfachen Text zum Senden in PSK.",
    "Load plain text for RTTY transmission.": "Lädt einfachen Text zum Senden in RTTY.",
    "Maximum AFC search window around each RTTY marker. Start with ±20 Hz; use smaller values for crowded contest bands.":
        "Größter AFC-Suchbereich um jede RTTY-Markierung. Mit ±20 Hz beginnen; in vollen Contest-Bändern kleiner wählen.",
    "Maximum AFC search window around the Hellschreiber marker. ±10 to ±20 Hz is a good starting point.":
        "Größter AFC-Suchbereich um die Hellschreiber-Markierung. ±10 bis ±20 Hz sind ein guter Anfang.",
    "Maximum AFC search window around the PSK marker. Start with ±20 Hz; use smaller values in crowded bands.":
        "Größter AFC-Suchbereich um die PSK-Markierung. Mit ±20 Hz beginnen; in vollen Bändern kleiner wählen.",
    "Maximum MFSK tone-bank AFC correction range around the selected center frequency.":
        "Größter AFC-Korrekturbereich des MFSK-Tonsatzes um die gewählte Mittenfrequenz.",
    "Maximum number of parallel RTTY shadow decoders. Lower this on older PCs.":
        "Höchstzahl paralleler RTTY-Nebendecoder. Auf älteren PCs verringern.",
    "Narrow dual-channel RTTY filter. Keeps only tight bands around Mark and Space and strongly rejects signals outside and between the two tones. Useful on crowded contest bands.":
        "Schmales Zweikanal-RTTY-Filter. Lässt nur enge Bereiche um Mark und Space durch und unterdrückt Signale außerhalb und zwischen den Tönen stark. Hilfreich in vollen Contest-Bändern.",
    "PSK audio carrier tone. 1000 Hz is a safe default for sound-card testing; center the signal on this marker in the waterfall.":
        "PSK-Audioträger. 1000 Hz ist ein sicherer Startwert; das Signal im Wasserfall auf diese Markierung legen.",
    "Paper speed / Hellschreiber column rate. Use this if received letters look horizontally compressed or stretched. Standard Feld Hell and FSK-105 use 17.5 columns per second.":
        "Papiergeschwindigkeit (Spalten pro Sekunde). Anpassen, wenn empfangene Buchstaben waagerecht gestaucht oder gedehnt wirken. Feld Hell und FSK-105 nutzen 17,5 Spalten pro Sekunde.",
    "Pulse the configured RTS PTT serial line for a short hardware test from Settings/diagnostics.":
        "Tastet die eingestellte RTS-PTT-Leitung kurz zum Hardwaretest.",
    "QSSTV-style SSTV clock calibration. It does not need an audio loopback: RX counts captured audio frames and TX counts frames accepted by the playback device, both against the PC monotonic clock. For a meaningful result, synchronize the system clock with NTP/timesyncd and let the measurement run for several minutes.":
        "SSTV-Taktkalibrierung wie in QSSTV, ohne Audio-Schleife: RX zählt aufgenommene, TX vom Ausgabegerät angenommene Audioframes, beide gegen die Systemuhr. Für ein brauchbares Ergebnis die Uhr per NTP/timesyncd stellen und mehrere Minuten messen.",
    "RMS input level in dBFS. Very low values indicate no audio or wrong input device.":
        "Effektiver Eingangspegel in dBFS. Sehr niedrige Werte bedeuten kein Audio oder ein falsches Eingangsgerät.",
    "RTTY symbol rate. Common amateur value is 45.45 baud; 50, 75 and 100 baud are also available for compatibility tests.":
        "RTTY-Schrittgeschwindigkeit. Im Amateurfunk üblich sind 45,45 Baud; 50, 75 und 100 Baud gibt es für andere Dienste und Tests.",
    "RX mode and receiver filter width. ~ marks a Hamlib value that may be nominal. MultiModeModem transmits RTTY and CW as audio; native FSK/CW keying is a separate radio function.":
        "Empfangsbetriebsart und Filterbreite des Funkgeräts. ~ kennzeichnet einen Hamlib-Wert, der nur nominell sein kann. MultiModeModem sendet RTTY und CW als Audio; FSK- und CW-Tastung im Funkgerät sind eine eigene Funktion.",
    "RX tone detector bandwidth for the visual Hell paper. FSK-105 uses two tones around the center marker.":
        "Bandbreite des Tondetektors für das Hell-Papier. FSK-105 nutzt zwei Töne um die Mittenmarkierung.",
    "Received MFSK text. MFSK16 uses standard Varicode/FEC; MFSK32 remains experimental.":
        "Empfangener MFSK-Text. MFSK16 nutzt Standard-Varicode/FEC; MFSK32 ist experimentell.",
    "Refresh visible audio and serial device lists after plugging/unplugging USB sound cards or radio interfaces.":
        "Aktualisiert die Audio- und Schnittstellenlisten nach dem Ein- oder Ausstecken von USB-Soundkarten oder Interfaces.",
    "Removed in v45/v46: slant correction is disabled to preserve the stable WEFAX line timing path.":
        "Seit v45/v46 entfernt: Die Schräglagenkorrektur ist abgeschaltet, damit das WEFAX-Zeilentiming stabil bleibt.",
    "Run lightweight parallel RTTY shadow decoders over the waterfall while the main terminal remains tuned to the selected signal.":
        "Lässt einfache RTTY-Nebendecoder parallel über den Wasserfall laufen, während das Hauptterminal auf dem gewählten Signal bleibt.",
    "Runtime status log. Use the dedicated runtime log button for longer CAT/FT diagnostics when debugging timing or PTT issues.":
        "Laufzeitprotokoll. Für längere CAT/FT-Diagnosen bei Timing- oder PTT-Problemen den Knopf für das Laufzeitprotokoll verwenden.",
    "SSTV RX/TX supports Martin, Scottie, Robot and PD families. VIS auto-detect selects the received mode when sync is enabled.":
        "SSTV-Empfang und -Senden unterstützen die Familien Martin, Scottie, Robot und PD. Mit eingeschalteter Synchronisation erkennt VIS die empfangene Betriebsart.",
    "Save the currently displayed MeteoFax image as a PNG file selected manually.":
        "Speichert das angezeigte Wetterfax-Bild als PNG unter einem selbst gewählten Namen.",
    "Select BPSK/QPSK symbol rate. Higher rates need stronger, cleaner and wider signals; QPSK variants in this build are experimental.":
        "BPSK/QPSK-Geschwindigkeit wählen. Höhere Raten brauchen stärkere, sauberere und breitere Signale; die QPSK-Varianten sind experimentell.",
    "Select a common RTTY baud/shift/tone preset. Use Custom for manual values.":
        "Übliche RTTY-Vorgabe für Baud, Shift und Töne wählen. „Custom“ für eigene Werte.",
    "Select a station example. The entry sets practical WEFAX defaults and shows example RF frequencies in the tooltip/status text.":
        "Beispielstation wählen. Der Eintrag setzt passende WEFAX-Werte und zeigt Beispielfrequenzen im Tooltip und in der Statuszeile.",
    "Select classic keyed-amplitude Feld Hell or two-tone FSK-105 Hellschreiber.":
        "Klassisches amplitudengetastetes Feld Hell oder Zweiton-Hellschreiber FSK-105 wählen.",
    "Select standard MFSK16 Varicode/FEC or legacy experimental MFSK32.":
        "Standard-MFSK16 (Varicode/FEC) oder das ältere, experimentelle MFSK32 wählen.",
    "Show the selected RTTY decoder text vertically between the Mark and Space tones on the waterfall. Disable it for an unobstructed spectrum.":
        "Zeigt den Text des gewählten RTTY-Decoders senkrecht zwischen Mark- und Space-Ton im Wasserfall. Ausschalten für ein freies Spektrum.",
    "Text to transmit as BPSK/QPSK. The transmitter uses Varicode and shaped phase changes.":
        "Sendetext für BPSK/QPSK. Gesendet wird mit Varicode und weichen Phasenwechseln.",
    "Text to transmit as CW. Unsupported characters are skipped or converted to spacing.":
        "Sendetext für CW. Nicht unterstützte Zeichen werden übersprungen oder als Pause gesendet.",
    "Text to transmit as Hellschreiber. Local TX is printed in red on the same paper tape.":
        "Sendetext für Hellschreiber. Die eigene Aussendung erscheint rot auf demselben Papierstreifen.",
    "Text to transmit as ITA2/Baudot AFSK RTTY. Unsupported characters are converted to spaces.":
        "Sendetext für RTTY (ITA2/Baudot, AFSK). Nicht unterstützte Zeichen werden zu Leerzeichen.",
    "Text to transmit. MFSK16 uses standard Varicode/FEC; MFSK32 uses the legacy experimental path.":
        "Sendetext. MFSK16 nutzt Standard-Varicode/FEC, MFSK32 den älteren experimentellen Weg.",
    "Track a common RX offset around the RTTY markers. AFC preserves the selected shift and never changes the TX tones.":
        "Folgt einer gemeinsamen Empfangsablage um die RTTY-Markierungen. Die AFC behält die Shift bei und ändert nie die Sendetöne.",
    "Transmit the CW text typed in the input box.": "Sendet den eingegebenen CW-Text.",
    "Transmit the Hellschreiber text typed in the input box.": "Sendet den eingegebenen Hellschreiber-Text.",
    "Transmit the MFSK text typed in the input box.": "Sendet den eingegebenen MFSK-Text.",
    "Transmit the loaded image using the active mode settings. Waterfall shows the generated TX audio; the image overlay shows transmitted rows.":
        "Sendet das geladene Bild mit den aktiven Einstellungen. Der Wasserfall zeigt das erzeugte Sendeaudio, das Bild die schon gesendeten Zeilen.",
    "Transmit the text typed in the input box.": "Sendet den eingegebenen Text.",
    "Stop the current RTTY transmission.": "Beendet die laufende RTTY-Aussendung.",
    "Stop the current PSK transmission.": "Beendet die laufende PSK-Aussendung.",
    "Stop the current MFSK transmission.": "Beendet die laufende MFSK-Aussendung.",
    "Stop the current CW transmission.": "Beendet die laufende CW-Aussendung.",
    "Stop the current Hellschreiber transmission.": "Beendet die laufende Hellschreiber-Aussendung.",
    "Ready. Use 180 s or more for reliable SSTV calibration.": "Bereit. Für eine zuverlässige SSTV-Kalibrierung 180 s oder länger messen.",
    "Transmit this quick reply. If TX is busy, queue it for the next transmission.":
        "Sendet diese Schnellantwort. Läuft gerade eine Aussendung, wird sie für die nächste vorgemerkt.",
    "WeatherFax line rate in lines per minute. Common HF WEFAX is 120 LPM; some signals use 60, 90, 180 or 240 LPM.":
        "Wetterfax-Zeilen pro Minute. Auf Kurzwelle meist 120 LPM; manche Sender nutzen 60, 90, 180 oder 240 LPM.",
    "Zoom the Hellschreiber paper on both axes. Existing RX/TX pixels are preserved; only the display scale changes.":
        "Vergrößert das Hellschreiber-Papier in beiden Richtungen. Vorhandene Zeichen bleiben erhalten, nur die Darstellung ändert sich.",
    "What's This?": "Direkthilfe",
    "Transmit macro: %1\n\n%2": "Makro senden: %1\n\n%2",
    "Type Hellschreiber text to transmit, then press ➤.": "Hellschreiber-Text eingeben, dann ➤ drücken.",
    "Quick reply — writable while the main TX buffer is transmitting": "Schnellantwort – auch während einer Aussendung beschreibbar",
    "Add text overlay": "Textüberlagerung hinzufügen",
    "Narrow Mark/Space filter": "Schmales Mark/Space-Filter",
    "Use 180 s or more for reliable SSTV calibration.": "Für eine zuverlässige SSTV-Kalibrierung 180 s oder länger messen.",
    "RX %1: marker %2 Hz → tracked %3 Hz · %4 · decode BW %5 Hz (set %6 Hz)":
        "RX %1: Markierung %2 Hz → verfolgt %3 Hz · %4 · Dekodierbandbreite %5 Hz (eingestellt %6 Hz)",
    "Set My Call and My Locator in Settings -> User/QTH": "Eigenes Rufzeichen und Locator unter Einstellungen → Benutzer/QTH eintragen",
    "NTP status unknown; verify PC clock before FT8": "NTP-Status unbekannt; vor FT8 die PC-Uhr prüfen",
    "Select or generate an FT8 message before starting TX.": "Vor dem Senden eine FT8-Nachricht wählen oder erzeugen.",
    "Some QSOs could not be saved. Check the logbook path and disk, then close again to retry. The application will remain open.":
        "Einige QSOs konnten nicht gespeichert werden. Logbuchpfad und Datenträger prüfen und erneut schließen. Das Programm bleibt offen.",

    # Settings and dialogs
    "Audio / PTT / CAT": "Audio / PTT / CAT",
    "Mark callsigns already present in the ADIF log with a strike-through line in RX/decode views.":
        "Rufzeichen, die schon im ADIF-Logbuch stehen, in Empfangs- und Decode-Ansichten durchstreichen.",
    "Choose a readability preset for FT/MSK/Q65 decode and message tables.":
        "Lesbarkeits-Vorgabe für die FT/MSK/Q65-Decode- und Nachrichtentabellen wählen.",
    "Select the global MultiModeModem colour palette.": "Farbschema von MultiModeModem wählen.",
    "Automatically point at the selected QSO (opt-in)": "Automatisch auf das gewählte QSO drehen (optional)",
    "PTT uses the Radio / CAT connection below.": "PTT nutzt die CAT-Verbindung zum Funkgerät (unten).",
    "MultiModeModem keeps the internal logbook as one ADIF file. Choose where that file is stored.":
        "MultiModeModem führt das Logbuch als eine ADIF-Datei. Hier wählen, wo sie liegt.",
    "The folder does not exist yet. MultiModeModem will try to create it when saving the logbook.":
        "Der Ordner existiert noch nicht. MultiModeModem legt ihn beim Speichern des Logbuchs an.",
    "Type part of the model name, e.g. 7300, FT-991, TS-590": "Teil des Modellnamens eingeben, z. B. 7300, FT-991, TS-590",
    "Optional: host:port, rigctld:host:port or HRD localhost:7809; leave empty for serial CAT":
        "Optional: host:port, rigctld:host:port oder HRD localhost:7809; leer lassen für serielles CAT",
    "Optional: host:port or rigctld:host:port; leave empty for serial CAT":
        "Optional: host:port oder rigctld:host:port; leer lassen für serielles CAT",
    "Hardware RTS/CTS": "Hardware (RTS/CTS)",
    "CAT not tested": "CAT nicht getestet",
    "CAT failed or not tested": "CAT fehlgeschlagen oder nicht getestet",
    "Suggested serial speed": "Empfohlene Baudrate",
    "Suggested Yaesu CAT speed: 38400 baud for many recent models. Match the CAT Rate/Computer Baud setting in the radio menu.":
        "Für viele neuere Yaesu-Geräte 38400 Baud. Muss zur Einstellung CAT RATE im Menü des Funkgeräts passen.",
    "power in watts": "Leistung in Watt",
    "Current mode only": "Nur aktueller Modus",
    "Grid and worked squares": "Locator-Gitter und gearbeitete Felder",
    "Custom dates (UTC)": "Eigener Zeitraum (UTC)",
    "Through UTC date": "Bis UTC-Datum",
    "One per DXCC country": "Eins je DXCC-Land",
    "One per locator square": "Eins je Locator-Feld",
    "Rotator: disabled in settings": "Rotor: in den Einstellungen abgeschaltet",
    "Rotator: not yet configured": "Rotor: noch nicht eingerichtet",
    "Rotator: connected": "Rotor: verbunden",
    "Rotator: disconnected": "Rotor: getrennt",
    "Qt SSL/OpenSSL is unavailable; online OSM HTTPS tiles will use the offline map unless M3_OSM_TILE_BASE points to a local HTTP tile proxy/cache.":
        "Qt SSL/OpenSSL fehlt; statt der OSM-Online-Kacheln wird die Offline-Karte genutzt, außer M3_OSM_TILE_BASE zeigt auf einen lokalen HTTP-Kachel-Proxy.",
    "OSM HTTPS tiles require Qt SSL/OpenSSL on Windows. Rebuild MXE Qt with OpenSSL or set M3_OSM_TILE_BASE to a local HTTP tile proxy/cache.":
        "OSM-HTTPS-Kacheln brauchen unter Windows Qt SSL/OpenSSL. MXE-Qt mit OpenSSL bauen oder M3_OSM_TILE_BASE auf einen lokalen HTTP-Kachel-Proxy setzen.",
    "HTTPS/TLS tile request failed. Verify that the Windows build has Qt SSL/OpenSSL, or set M3_OSM_TILE_BASE to a local HTTP tile proxy/cache.":
        "Abruf der HTTPS-Kacheln fehlgeschlagen. Prüfen, ob der Windows-Build Qt SSL/OpenSSL enthält, oder M3_OSM_TILE_BASE auf einen lokalen HTTP-Kachel-Proxy setzen.",

    # Logbook, contest export
    "Missing or invalid recorded exchange field: %1": "Austauschfeld fehlt oder ist ungültig: %1",
    "Select QSOs from one contest edition and station": "QSOs eines Contest-Durchgangs und einer Station auswählen",
    "Band not allowed by contest profile": "Band im Contest-Profil nicht erlaubt",
    "Mode does not match contest profile": "Betriebsart passt nicht zum Contest-Profil",
    "Invalid or missing station/contact callsign": "Eigenes oder Gegenstations-Rufzeichen fehlt oder ist ungültig",
    "Sent CQ zone missing or invalid": "Gesendete CQ-Zone fehlt oder ist ungültig",
    "Received CQ zone missing or invalid": "Empfangene CQ-Zone fehlt oder ist ungültig",
    "US state missing or invalid": "US-Bundesstaat fehlt oder ist ungültig",
    "Canadian province missing or invalid": "Kanadische Provinz fehlt oder ist ungültig",
    "RTTY mode required": "Betriebsart RTTY erforderlich",
    "Band not allowed for CQ WW RTTY": "Band für CQ WW RTTY nicht erlaubt",
    "RST missing or invalid": "RST fehlt oder ist ungültig",
    "Frequency missing or inconsistent with band": "Frequenz fehlt oder passt nicht zum Band",
    "Station callsign missing": "Stationsrufzeichen fehlt",
    "Export contest QSOs to Cabrillo": "Contest-QSOs als Cabrillo exportieren",
    "1. Select the contest edition": "1. Contest-Durchgang wählen",
    "Editions found in the complete logbook, independent of the current search filter. Dates are UTC; different station callsigns are kept separate.":
        "Im ganzen Logbuch gefundene Durchgänge, unabhängig vom Suchfilter. Daten in UTC; verschiedene Stationsrufzeichen bleiben getrennt.",
    "No contest editions found. Records need a contest identifier or rule and a valid UTC date. Untagged contacts are recognized only when dates, mode and recorded exchanges identify one edition.":
        "Keine Contest-Durchgänge gefunden. QSOs brauchen eine Contest-Kennung oder Regel und ein gültiges UTC-Datum. QSOs ohne Kennung werden nur erkannt, wenn Datum, Betriebsart und Austausch eindeutig zu einem Durchgang passen.",
    "Sessions from the same edition are combined for every contest. Recognition uses logged contest/rule tags, available schedules and recorded exchanges. Untagged matches are marked in the preview; ambiguous contacts are not assigned automatically.":
        "Sitzungen desselben Durchgangs werden zusammengefasst. Erkannt wird über geloggte Contest-/Regelkennungen, bekannte Termine und den Austausch. Treffer ohne Kennung sind in der Vorschau markiert; mehrdeutige QSOs werden nicht automatisch zugeordnet.",
    "Not selected: %1 records without a valid UTC date; %2 outside known contest periods; %3 with ambiguous contest membership.":
        "Nicht ausgewählt: %1 ohne gültiges UTC-Datum, %2 außerhalb bekannter Contest-Zeiten, %3 mit unklarer Contest-Zugehörigkeit.",
    "2. Station and entry category": "2. Station und Teilnahmekategorie",
    "Read contest exchanges automatically": "Contest-Austausch automatisch lesen",
    "Sent exchange ADIF field": "ADIF-Feld für gesendeten Austausch",
    "Received exchange ADIF field": "ADIF-Feld für empfangenen Austausch",
    "Exchanges follow each contest profile using recorded internal fields, standard ADIF fields and exchange strings. Manual ADIF field selection is available if needed. The band category does not remove off-band QSOs.":
        "Der Austausch folgt dem jeweiligen Contest-Profil und nutzt interne Felder, Standard-ADIF-Felder und Austauschtexte. Bei Bedarf lassen sich die ADIF-Felder von Hand wählen. Die Bandkategorie entfernt keine QSOs auf anderen Bändern.",
    "3. Review QSOs before export": "3. QSOs vor dem Export prüfen",
    "Only checked QSOs will be exported. Missing or invalid data is shown for each row and blocks export until corrected in the logbook or explicitly excluded here. The logbook is not changed.":
        "Exportiert werden nur angehakte QSOs. Fehlende oder ungültige Angaben stehen in jeder Zeile und verhindern den Export, bis sie im Logbuch korrigiert oder hier ausgeschlossen sind. Das Logbuch wird nicht verändert.",
    "Recognized from date and exchange": "Anhand von Datum und Austausch erkannt",
    "%1 of %2 QSOs selected; %3 with invalid or missing data.": "%1 von %2 QSOs ausgewählt; %3 mit fehlenden oder ungültigen Angaben.",
    "This choice applies to ADIF, CSV, copy, print and PDF.": "Diese Auswahl gilt für ADIF, CSV, Kopieren, Drucken und PDF.",
    "Search callsign, locator or any field...": "Rufzeichen, Locator oder beliebiges Feld suchen …",
    "Custom UTC interval": "Eigener UTC-Zeitraum",
    "Until UTC (excluded)": "Bis UTC (ausschließlich)",
    "No QSOs match the current filters.": "Keine QSOs passen zu den Filtern.",
    "ADIF logbook (*.adi);;ADIF logbook (*.adif);;All files (*)": "ADIF-Logbuch (*.adi);;ADIF-Logbuch (*.adif);;Alle Dateien (*)",
    "Save Cabrillo log": "Cabrillo-Log speichern",
    "Cabrillo log (*.log);;All files (*)": "Cabrillo-Log (*.log);;Alle Dateien (*)",
    "Cabrillo log exported successfully.": "Cabrillo-Log exportiert.",
    "MultiModeModem logbook statistics": "MultiModeModem-Logbuchstatistik",
}

# Former word-by-word artefacts ("+ Add bis log", "wartet for CQ"), now
# reviewed.  Amateur-radio terms stay as operators use them in German:
# Tune, Slot, Log, QSO, CQ, RX/TX, PTT, CAT, Decodes.
DE.update({
    "Standard 360°, stop at North (N-E-S-W-N)": "Standard 360°, Anschlag im Norden (N-O-S-W-N)",
    "Standard 360°, stop at South (S-W-N-E-S)": "Standard 360°, Anschlag im Süden (S-W-N-O-S)",
    "Usually noisy/importer-specific ADIF field. Hidden by default but can be enabled here.": "Meist unwichtiges oder importspezifisches ADIF-Feld. Standardmäßig ausgeblendet, kann hier eingeblendet werden.",
    "Please choose a logbook ADIF file path.": "Bitte einen Pfad für die ADIF-Logdatei wählen.",
    "Please include a file name, for example logbook.adi.": "Bitte einen Dateinamen angeben, z. B. logbook.adi.",
    "PTT TX test cancelled — no TX command sent.": "PTT-TX-Test abgebrochen – kein TX-Befehl gesendet.",
    "off standard FT frequency": "neben der FT-Standardfrequenz",
    "Auto QSO: waiting for CQ": "Auto QSO: wartet auf CQ",
    "System clock synced; UDP NTP still retrying": "Systemuhr synchron; UDP-NTP versucht es weiter",
    "Decodes used": "Verwendete Decodes",
    "FT decode list cleared.": "FT-Decode-Liste geleert.",
    "Open FT8/FT4 WAV test file": "FT8/FT4-WAV-Testdatei öffnen",
    "Logbook file changed:": "Logdatei geändert:",
    "Settings file:": "Einstellungsdatei:",
    "Pending FT digital TX cancelled.": "Wartendes FT-TX abgebrochen.",
    "SSTV image reset.": "SSTV-Bild zurückgesetzt.",
    "MeteoFax image reset.": "MeteoFax-Bild zurückgesetzt.",
    "Auto-save disabled; completed image kept on screen.": "Automatisches Speichern aus; das fertige Bild bleibt angezeigt.",
    "FT timing: handing prepared waveform to dedicated FT TX worker thread.": "FT-Timing: vorbereitetes Signal geht an den FT-TX-Worker.",
    "FT TX pre-arm cancelled: no pending FT message.": "FT-TX-Vorbereitung abgebrochen: keine wartende FT-Nachricht.",
    "FT TX cancelled: no pending FT message.": "FT-TX abgebrochen: keine wartende FT-Nachricht.",
    "FT frequency: TX marker prepared at %1 Hz (%2).": "FT-Frequenz: TX-Marker auf %1 Hz vorbereitet (%2).",
    "FT timing: %1 %2 armed by UTC scheduler; next selected slot in %3 ms, PTT/audio pre-arm %4 ms before boundary: %5": "FT-Timing: %1 %2 vom UTC-Scheduler vorbereitet; nächster gewählter Slot in %3 ms, PTT/Audio %4 ms vor Slotbeginn: %5",
    "FT caller queue: cleared %1 waiting caller(s): %2": "FT-Anruferliste: %1 wartende Anrufer entfernt: %2",
    "Starting %1 tune at %2 Hz.": "Starte %1-Tune auf %2 Hz.",
    "Audio capture started at %1 Hz.": "Audioaufnahme mit %1 Hz gestartet.",
    "TX %1 ON on %2.": "TX %1 EIN auf %2.",
    "TX audio started at %1 Hz%2.": "TX-Audio mit %1 Hz gestartet%2.",
    "CAT TEST: queued on isolated worker.": "CAT-TEST: im separaten Worker eingereiht.",
    "Send selection to log: Callsign": "Auswahl ins Log: Rufzeichen",
    "Send selection to log: RST sent": "Auswahl ins Log: RST gesendet",
    "Send selection to log: RST received": "Auswahl ins Log: RST empfangen",
    "Send selection to log: Band": "Auswahl ins Log: Band",
    "Send selection to log: Mode": "Auswahl ins Log: Mode",
    "Send selection to log: Grid square": "Auswahl ins Log: Locator",
    "Tune": "Tune",
})
DE.update({
    "Force WEFAX RX now": "WEFAX-RX jetzt erzwingen",
})
