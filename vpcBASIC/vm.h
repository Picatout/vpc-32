/* 
 * File:   vm.h
 * Author: Jacques Deschênes
 *
 * Created on 31 mai 2013, 16:40
 */

#ifndef OPCODES_H
#define	OPCODES_H

#define RAM_SEG  0xA000
#define CODE_SEG 0x9D00
#define SFR_SEG  0xBF80
#define CFG_SEG  0xBFC0



// op codes
#define IEND 0
#define IQRX (IEND+1)
#define IEMIT (IQRX+1)
#define ILIT (IEMIT+1)
#define ICALL (ILIT+1)
#define IRET (ICALL+1)
#define IBRA (IRET+1)
#define IQBRA (IBRA+1)
#define IEXEC (IQBRA+1)
#define ISTORE (IEXEC+1)
#define IFETCH (ISTORE+1)
#define ICSTORE (IFETCH+1)
#define ICFETCH (ICSTORE+1)
#define IRFETCH (ICFETCH+1)
#define IUFETCH (IRFETCH+1)
#define IUSTORE (IUFETCH+1)
#define ITOR   (IUSTORE+1)
#define IRFROM (ITOR+1)
#define IDROP (IRFROM+1)
#define IDUP (IDROP+1)
#define ISWAP (IDUP+1)
#define IOVER (ISWAP+1)
#define IPICK (IOVER+1)
#define IPLUS  (IPICK+1)
#define ISUB  (IPLUS+1)
#define ISTAR  (ISUB+1)
#define IUSTAR (ISTAR+1)
#define IMSTAR (IUSTAR+1)
#define IUMSTAR (IMSTAR+1)
#define ISLASH (IUMSTAR+1)
#define IUSLASH (ISLASH+1)
#define IMOD (IUSLASH+1)
#define IUMSMOD (IMOD+1)
#define IMSMOD (IUMSMOD+1)
#define ILTZ (IMSMOD+1)
#define IZEQUAL (ILTZ+1)
#define IAND (IZEQUAL+1)
#define IOR  (IAND+1)
#define IXOR  (IOR+1)
#define INOT  (IXOR+1)
#define IKEY  (INOT+1)
#define ISFR  (IKEY+1)
#define ISET  (ISFR+1)
#define ICLR  (ISET+1)
#define ITGL  (ICLR+1)
#define ICLIT (ITGL+1)
#define IWLIT (ICLIT+1)
#define IDOTQ (IWLIT+1)
#define ITICKS (IDOTQ+1)
#define IDELAY (ITICKS+1)
#define IQDUP  (IDELAY+1)
#define IPLUS1 (IQDUP+1)
#define IMINUS1 (IPLUS1+1)
#define IPLUSSTR (IMINUS1+1)
#define I2STAR (IPLUSSTR+1)
#define I2SLASH  (I2STAR+1)
#define IDDROP  (I2SLASH+1)
#define IROT  (IDDROP+1)
#define IDDUP (IROT+1)
#define IMIN (IDDUP+1)
#define IMAX (IMIN+1)
#define IABS (IMAX+1)
#define ILSHIFT (IABS+1)
#define IRSHIFT (ILSHIFT+1)
#define IDIVMOD (IRSHIFT+1)
#define IQBRAZ (IDIVMOD+1)
#define IDCNT (IQBRAZ+1)
#define IRCNT (IDCNT+1)
#define IEQUAL (IRCNT+1)
#define INEQUAL (IEQUAL+1)
#define ILESS  (INEQUAL+1)
#define IGREATER (ILESS+1)
#define ILTEZ (IGREATER+1)
#define IGTEZ (ILTEZ+1)
#define IXFETCH (IGTEZ+1)
#define IXSTORE (IXFETCH+1)
#define IXLOOP (IXSTORE+1)
#define IPXLOOP (IXLOOP+1)
#define IUSER (IPXLOOP+1)

#define LASTOP IUSER


#define TOK_COUNT (LASTOP+1)
#define IBADOP -TOK_COUNT



#define CELL_SIZE 4






#endif	/* OPCODES_H */

