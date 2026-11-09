# CatRotator upstream attribution

MultiModeModem v4.13n begins integrating CatRotator ideas as a native MM rotator module.

Upstream package supplied by the user: CatRotator-main.zip.
CatRotator is GPLv3+ Qt/Hamlib software.

MultiModeModem does not embed CatRotator's bundled Hamlib DLLs. The integrated module uses MultiModeModem's existing Hamlib build and packaging path.

The standalone CatRotator WSJT-X/AirScout/PreviSat UDP tracking model is intentionally not carried into the integrated MM UI. In MultiModeModem, rotator tracking is driven by MM's selected QSO target and manual operator commands.
