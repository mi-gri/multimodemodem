"""Product name of this fork, see utils/ProductName.h.

MultiModeModem (m³) is based on MadModem by Lucian-Ioan Papadopol, IZ6NNH.
Help sources keep the upstream wording; the visible name is replaced when the
pages are written, so upstream help changes still merge.
"""
import re

LONG_NAME = "MultiModeModem"
SHORT_NAME = "m³"
_SHORT_RE = re.compile(r"(?<![/\\\w.])MM(?![/\\\w])")


def brand(text: str) -> str:
    return _SHORT_RE.sub(SHORT_NAME, text.replace("MultiModeModem", LONG_NAME))
