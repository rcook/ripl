set(RIPLMAINMAIN ${SRC}/riplmain)

add_library(riplmain STATIC
  ${RIPLMAINMAIN}/Error.cpp
  ${RIPLMAINMAIN}/Error.h
  ${RIPLMAINMAIN}/Image.h
  ${RIPLMAINMAIN}/Ranges.h
  ${RIPLMAINMAIN}/defs.h
  ${RIPLMAINMAIN}/ripl.h
  ${RIPLMAINMAIN}/riplallc.cpp
  ${RIPLMAINMAIN}/riplallc.h
  ${RIPLMAINMAIN}/ripldbug.h
  ${RIPLMAINMAIN}/ripldefs.h
  ${RIPLMAINMAIN}/riplfilt.cpp
  ${RIPLMAINMAIN}/riplfilt.h
  ${RIPLMAINMAIN}/riplgrey.h
  ${RIPLMAINMAIN}/riplimg.h
  ${RIPLMAINMAIN}/riplmisc.cpp
  ${RIPLMAINMAIN}/riplmisc.h
  ${RIPLMAINMAIN}/riplmsg.cpp
  ${RIPLMAINMAIN}/riplmsg.h
  ${RIPLMAINMAIN}/riplpars.cpp
  ${RIPLMAINMAIN}/riplpars.h
  ${RIPLMAINMAIN}/riplpbm.cpp
  ${RIPLMAINMAIN}/riplpbm.h
  ${RIPLMAINMAIN}/riplrgb.cpp
  ${RIPLMAINMAIN}/riplrgb.h
  ${RIPLMAINMAIN}/validate.h
)

