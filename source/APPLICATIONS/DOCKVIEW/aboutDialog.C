/****************************************************************************
** Form implementation generated from reading ui file 'aboutDialog.ui'
**
** Created: Thu Jan 6 16:25:31 2005
**      by: The User Interface Compiler ($Id: aboutDialog.C,v 1.1.2.2 2005/01/06 15:27:55 leonhardt Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "aboutDialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"199 196 57 1",
"2 c #181c18",
"X c #202020",
"0 c #202420",
"U c #292829",
"W c #292c29",
"s c #313031",
"g c #313431",
"h c #393839",
"l c #393c39",
"q c #414041",
"f c #414441",
"b c #4a484a",
"r c #4a4c4a",
"i c #525052",
"A c #525552",
"y c #5a595a",
"a c #5a5d5a",
"o c #626162",
"D c #626562",
"m c #6a696a",
"x c #6a6d6a",
"t c #737173",
"C c #737573",
"H c #7b797b",
"p c #7b7d7b",
"I c #838183",
"G c #838583",
"# c #8b898b",
"c c #8b8d8b",
"z c #949194",
"F c #949594",
"V c #9c999c",
"J c #9c9d9c",
"M c #a4a1a4",
"v c #a4a5a4",
"N c #acaaac",
"K c #acaeac",
"L c #b4b2b4",
"u c #b4b6b4",
"j c #bdbabd",
"P c #bdbebd",
"Q c #c5c2c5",
"E c #c5c6c5",
"w c #cdcacd",
"k c #cdcecd",
"n c #d5d2d5",
"R c #d5d6d5",
"B c #dedade",
"S c #dedede",
"T c #e6e2e6",
"e c #e6e6e6",
"d c #eeeaee",
"O c #eeeeee",
"1 c #f6f2f6",
"Z c #f6f6f6",
"Y c #fffaff",
". c #ffffff",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".........................................................................................#abacd........................................................................................................",
".......................................................................................efgghhhij.......................................................................................................",
"......................................................................................kfghllllmhn.......#oggp..........................................................................................",
"......................................................................................aghqqqfgirf.....kfghlbaf.........................................................................................",
".....................................................................................eslqqfibbqqgtuvuwxsshlbyra........................................................................................",
".....................................................................................vghffizAfgifiAaaaaaaaqqfqqk.......................................................................................",
"..................................................................................BvomqqqbiCrbmxDoaomooooyribhlp.......................................................................................",
"...............................................................................EFCxCp#yqqbmwxiCxCtxDaoooooybighCGHjB...................................................................................",
"............................................................................EF#apIGJujcllbxKirIHHHHHHDoooyyqqqgLFGpIMNwO...............................................................................",
".........................................................................BvHtx#cMPEPujzlqfbqrap#CxCIHHHDDoAfbgHdeELFGHIFNO.............................................................................",
"......................................................................Bv#apIGMPEwnQv#IHillbfbrmtx##GIHIGttxoolDcvndeBvM#IIJKj..........................................................................",
"....................................................................JCxCIzvukRBSKzGHIHNzghqllbffxDIGtcIIGtcmmmaoHtcvnedkHH#GGHLj.......................................................................",
".....................................................MCGHj......kPCxHHzMQReTwuzGHIcMB...EUghssbaHHDCGHGHIcp#CCtxDaxHHptfgssgtcIzIJwO...ecobaH..........................................................",
"...................................................eysghqlad.Ojcmm#IJjBSTeQJC#IIFB.....MshllhaoHptxootp#C#GGGIp#CComHghqlggabDKVzIcMFKPrlqfAfsn........................................................",
"..................................................eyshllllrfVxDI#JunTTTQv#IIINE.......MhqqbimtHpCtxH..LIGtcGHIcppHHDhghslqfhhifkPEcIzzAllhWhqss........................................................",
"..................................................tsllqfhrxao#VNEReOjN#GGGvB..........ogbirxHHooo#e.....kztx##G#IIIrlhgffhqsllUxVKjjvVyffhghUsge.......................................................",
".................................................esghqqbifbAajBSeTMc#HpVE............vgfbrptxoatE.........SKIIIG#JxghqfiAffhgggqH#GFv#ffrmrbirXM.......................................................",
".................................................uWhqqb#abbqrSENzptGKP.............vxCqfbmJaaald............djptGImhgfffxHbqrWWhpIxHIabbqfbAfaac.......................................................",
".................................................uWllrrmmbAqqDGpx#S............dM#H#GjcqqbpllhC................uJCAfffrbaDfisgWWomHooybfgWbbyyAavO.....................................................",
".................................................ksllfbpBAffqqKP.Y..........kQc#IJLeZEplqfhrsgM..................SDlqfAHHbbhlbU0yyxDoooyArbibbyArDvO.jobgtn............................................",
".................................................VWhaooyMyqqllY...........jFIG#uRZwM#HxsllUghsoB..................vgffizIallhWsgytxoxDIomomomorbiaaaqhhifxfb...........................................",
"................................................Lrlitp#CbqrWhC.........kCx#VPd.OJC#xJw.jfgsWhamp..................vllrmxaaqfbshlmxtpmxaxDmmaDomtoaagssbhlbUha..........................................",
"...............................................JrabmCpLFrlqqsfgrj..ePcIzzPSdkLFCp#eY......knDComc..................rlDox#arbsl0qqtpDomCtxHmtoxaxaxDoDffisffhgk.........................................",
".........................................kHHpVcrbiatpN#trfggqllhfxucIzuR1SE#HGJj............comoou.................ebmCxJal0X0jvpAaDCottxDmtpDtpDtmooogbbglqUt.........................................",
"........................................MhllUbAaoAmtzMImsl0sllfAxf#Nw.TLFCCMPd...............pDomCd.................cmoH#yfXhkY...nTJziriiribafyoootxoxcrfgsWq.........................................",
".......................................jsgqqfqDCotxCKzCxramptArDrbweE#IILFS..................dtDomC.................cmtHCAsIe..........kNzGllbffrabimxcmbqrWhh.........................................",
"......................................esgfbrAxytrapFKptxpHHtcpMalbiImvwO......................Etmoov...............uaoHpxa.................plqfhrsbbhCSDqqbWha.........................................",
"......................................jhqslaoAooot#VzIcxCttxxCKCrqfuO..........................womomL..............MaHHHDD.................ebghqtMU0sllfqfgg0n.........................................",
"......................................esgfiaaaaotxvM#oootDomaaqfbgW.............................cmmoodk............Httx#yM..................HgfisnlU000fbgWsiB.........................................",
"......................................esgfiiJryDc#NGtmtpDzGhhghqfs#.............................dyDDoAghvO........BmoHGtiQ..................SlqfhJC20U0sgqqGHoO........................................",
".......................................obllrrrmmpJFIHd.Y...LfgWshAe..............................dxoobhhgv........vHtx#yAd...................illrr0U20U2haZuzxHd.Y.....................................",
".......................................DhhifqqxHIvGtS.Y.....MfisI.................................vqqgUgU0d.Y.....ttxxpaC....................NqqbWAsgWbAqtuSKHpV.......................................",
"......................................MtFvggqmtzMcIG........IootQ.................................iUgUX000GB.....TmtHCtAL....................illqfhXhi...M#RkVxtu......................................",
".....................................QmpSwxliCGcM#GS........IDomJ.................................iX222X20ramzu..LxCtIoAd...................vWbhgfXXXq...eGH1PcpHO.....................................",
"....................................dtCj.GHLmpGv#IV.........JaxDE................................db0U22020qqbAmH#GtpmpDi..................nvxX0XU0s2X0....nGFONzxcd.O..................................",
"....................................MCJ.VCwotGJJ#Hd.........jDCoL...............................dCr0XU202WhqggqmxDmtIma#...............ucibilU0s22X202.....LIjnvHtz....................................",
"...................................BtpejFxCtI#N#HN..........KixDEY.............................dxbAq0020x.jfhlbUaCCp#DoJO...TjpP....ntryfAffqUsX022XXC......GpBSzGDB...................................",
"...................................DGQTGtSaCCJz#Hd..........KxDmQ.............................RbbyAiQwud....dvGliCtpmabDHHKjUsXllw#ybiriribttqqX02X0haZY....eGGSuzxH.Y.................................",
"..................................FCNYMtPcpIFMcIj...........#oxDE............................RoaallwY...........DCotxaotttxfWWhaAfabqrAfaC#cRZGsl0sghqG......jIVTM#xC..................................",
".................................LmFBKixtrx#cVG#Y...........vooowO..........................dyibhae............kaaamxabbyAissghlbyqyoDoJjB.....knQmbhhiV......G#ujzGxu.................................",
"................................dyiz#shllhhiVztS.Y..........#xHawO.........................RiribaR.............#oxaaaaRKzqq2X2iriooo#j.Y...........CrqfiQ......G#BuzIDO................................",
"................................HyDghslhgffbACu.............cmood.........................BrfiiId............ScfgWramr...dr2XsllyrV.................HJzoo1.....kzzRNzptO...............................",
"...............................#oxb0ghqlbbytrqwO............#xDod.O......................QrfibbB...........eyshqslagh#....LWghqlgq..................vxEFtp......jIvuJzo#...............................",
"..............................JyrishllhbAqrmrfF............jsDobm.......................wobAf#............BrfghlbUaarqe....tsgWsge...................ttnIHc......M#LjzGmuO.............................",
"..............................mmtrU0sbqrAiribqfY..........k0Uqqbgt.....................QibirVY............rbsqqbWllUbWa.....jirib....................Bm#uCpL......G#LvHtzd.............................",
".............................HyooghqlbrVHilbAqhn..........mX00sgWstzvkBS1..........KxqqfyfAI.............MUqqbWlfWqfgWge.....JCDod....................QxcMCDO.....kCFNNGCxd.Y..........................",
"............................EArHighUqqfymuHttAsQ..........hX20iXslltpmptmtyFzvwEEEHWWgiaibI..............hqslagAaqqsghso.....kCxDEY....................NGN#tV......J#VKVImF............................",
"............................HAxyAfU0sbqfmFtxoxmM..........o2X2WhX0fbrAAaDAxyxDaxxDaWgsibqVY.............ElqfAffxzrbsqUsX.....doat#.....................eGHLImv......G#VVzIDj...........................",
"...........................vyyAyfbghfllhfxooHHHD#.........jX200U22qfbAffrbrfiboaaArslilU0n...........ekVxDqqbifmtffhggWW......#Gm#.......................H#KHtQ.....dtzM#GImB..........................",
"..........................wC#zArJwggqlqfbAxCtmtHCtk.......rhgX200J.TnjQJVzmhrbibfbrU0sU0sY.........n#GGGCpqqb#Etllhfghs0......Gttx.......................wovHtmO.....wCFvVGxCuJud.Y....................",
"........................edpHPkabRofgWsqqbAyyxxHaCxCV.....tqfgqqDu.............dnQNUgUXU0sY.....dSFGCx#IGMLfbrpBxbqrWhhXx......kxcCS.......................cmkcoG......jIvVzplllibI.....................",
".....................kabqgxMBzGnAmHb0gghflfbDomCttxxpn..EqqgUR....................iUX02Xu....wNptGI#VzQPucrffbribhqqsg0J......dtGpM........................#IJGxj.Y....JpFGfbriyfXre...................",
"....................jsghqlg#NpjmooopafWWsl0mooopmtHCttzBllhfz......................VhgXlR.TFIHpHcMJjwNJFIJmhbbhbAqhhhgWK.......tp#N........................Rx#JCDE.....dtarqqqfqqqst...................",
"...................kfWqqqfqDrGFDoxIGtmwkLjd.OtAaatpmptCAilliB........................yripcmG##IuQQQFGzGIHItqqbAhlbUhgxxDO......JIHcY........................EmFcmGJun...rlqfiAfqqsg0n..................",
"...................MsllbffrarqomHCxCxJ........JAmomHCC#Cxyfd.O......................BGHo#cVzNEEQJzGIzIHHDxJallhqqbWAN#txH......M#IG..........................vxVCshlloBRhgffbmtfllhWu..................",
"...................yfghqcGyriytxCtIxJ..........domomtxoxcxC......................dPcpIGIFNQTkLVGz##IIDavRZ.TmlqUghquRuzxH#.....jIHHj.........................eCrqflagsDjhbACPHoffhggL..................",
"...................yhqrxxCtfimmtIGtcY...........dVyDCotxp#CCj....d............ePJCIG#zMESwwMVzc#IHttcd.......BPC#IIIRQJGxou.....C#Cn..........................ofllbffghCrqfmMArsllUgL..................",
"...................MUqbiICxyDopIxtG...............jooot#xHHoDGR#bqgsoj......jcIHHz#NEB1eQPzIcxImHMBS1.............MIJdLFGtIO....IHcI..........................rlhyfllUgqqbWAbbyllUgXe..................",
"...................esgoat#rxxHHHHHQ................uaaooCxtmtfWghqlbrgxeTkHHpI#IJETT.eQJ#VIHHHFv...................#GvTuzxH#O...JCIp..........................ghqGzAmmqVWhqaCCtrlqUI...................",
"....................joopIAmoCp#DDRZ................arqooatyflsllfqqqDxgHHHc#IMPdO..wNJG##xDIE......................dC#jEN#Iou...TmcxJ.........................obhrbmooopnhqH#GtmhXie...................",
"....................KitHoaottCxtS.Y...............GyhlbyoaaAWgsqqbifbaDqcMJLkd..EF#z##xDax#.........................BG#SKzCHywO.KIImfxO.......................vhlbfDoDCxCVbHHz#ybA.....................",
"....................CACtACttxxpn.................jal0hk.GrilUlqfhrfbriylCPkZORMcIFIHopu.tp#n.........................KHFBKVGxC.kghfaol#.......................GfWqqfxmmtIxJG##GtllY....................",
"...................RiAaDAttxIHL..................yDglv...VyUgUfiAaaaqhbiITQuzc#IHxHud...pI#K..........................vGvNMcIDGHigDofiA......................tpmaWacvDoDCxaIV#Iofbn....................",
"...................#ybAmtHCttz...............eQxooffh.....MhlllbfccyfilbqVcGGpxVwO......JI#I...........................GpJzvHtmAsrVHfbDT....................QxIHDv...dtattxxpFIabbP....................",
".................jtqhaAttxIxDEY.............ElXXXllheZ....jU0sfbrAuwmhrffxtmGEB.........ScIIB..........................EG#NzMcIDfbrqqIFGJO.................dttxxM......CDoxIcGGyriz....................",
"...............ufgWsqmxtCxCxou.............dA0WXXX2x......LsllfqqiJjorlhllL..............G#IFd.Y........................NGIFVzpmoghqlpI#FIvT...............J#HHDO......exoC#cIHalblCn..................",
"..............jsgllbDCotxpAxyM.............jsh2X2W2X.......U0sbqbqrAifisfUY..............vGpxhv..........................v#JVzpmtfhqrVGpFc#Ij.Y...........kapDiS.........IDcGGptllrWWn.................",
".............RqqgqqbiCpCtAaatmO..........kNAffqU2X2W.......rhghflffbgbqgs#...............mrfiogd..........................GIFMFGCm#.Y.ectpNc#IjO..........tHpCL..........ecIzztAfisfUfe................",
".............GWWhfiAatpDtfimom1.......Bzmboaals2X2Ws.......#UgUfqqfqqqfgWd.Y............JhhiCrqt........................dztIHVzFCHtRZ...eVGpVGzGQO......YIHItC............H#GztrqbbhU0c................",
".............U0fbgDGxaxDqqbDoDuO...ePxDmtaoAqqs2X2WNY.......JWWhfhgffbslk...............ElbmCrfH#KReO..................SIHcIGcczGptx......kcGGVzcIE.....LxHxan............IzGIHigrlhl0rY...............",
".............WWsffrCnMhllffxaooCdkpDiDobAaqbFf202X2R.........IWWsfghUq0u................MlqfAfgwnPKJwK1...............eImHHHGpJcIzCx#.......Bzp#VVFzeT#ImxtCj.............#ccztrlbUgUXA................",
".............tqhlqfAaaaqhbhAaDAibirDobAiiANE.Tjo0Uqo..........kfo#QcmokY................nxbglgDGxJQRkVvMPS...........vxCxCV..IHcIGGxoj.......dFGIGMVCsfbpHDG.............dtzGpHoU0s22WG................",
".............c0WllhfgbqrWhhitHDaqfmrfiopn.......yghsR..........mIV1jFx.................eIPcfDGS1wNDC#PEwjv#uQ...eV#RIHItCQ...nIz##zGmoO........ScIGcghftpman.............ScIGztbU0XU2Ue................",
".............RlqUgfisfqqgUsgyImaqqbiCPd.........jsllc..........DIFdj#Hd................pLjmO.....Y..MvG#NkRNFGVCsfqyHHDxQ.....VGczIcxIp..........TFbqqxoooh#.............kGJJIalUgUXUu.................",
"..............w0WlghqlgqUsXllRxbqfFBY............ilUl..........oGJOPCG................ucPHP............kVx#NkLjiffxaCxtS.......#IV#ccxaI...........bqf#JfgWC.............dtzcIHaUqWfkY.................",
"...............dqqbAyhhxVWhqgfisfUe..............vllhz.........tHzdjzt...............BHRzvY...............EtJ#Vlq#allV.........BHIIIcIHaj..........CllrrrllJ.............jcIzztbbHb#...................",
"................eaHjSFV.esgWhqgfW#...............dlqfA.........IHVdEpC...............MKLpd.Y................dECllmDfWJ..........kcGFG##xoO.....Y...dghqfshhd.............QcFIHoAaxf#Y..................",
"................Ropjecv..LrU0sf0q.................usgWK........Iz#eEcx..............jFnDT.....................QshllhUk...........KxcIzCHHHO.........Jbglgfd.Y............JI#VCxbiCAm...................",
".................yIPTGV...exoVyI...................rrbm........IDcejcx.............dtEcF.......................tghUWa.............MI#IIItmG..........kGBxw...............ucFGCmaotA#...................",
".................acKTGV....Jmkpj...................crfHj.......accSwcx............ecQv#e........................VxoV..............eIG##GHpCL.........kIeCJ...............J#VzIaxoCA#...................",
".................yIuRJj....aonGv...................ebAmH.......#IcSjFx............vvuIeY........................NGG#Y..............eIGGczxDaxxCuY....jpkcE...............GtxoxmDc#rx...................",
".................yIuYMM....pCjpF....................NbHpkY.....mpGkLVx...........PcR#u..........................vz##................FGC#HGCAqqbWltO..wIdtE.............eHffrbhhijcmt...................",
"................eapM.#u....MmkIV....................BmmFcd.....zGInQ#Hd.........BzkVH.Y.........................v#v#.................Iz#Gtiqqfymilt..jIkcK1...........eilbqimxfgrcioO..................",
".................aHKSzv....ttkHJ.....................#m#pn.....ptGEEHH.Y........JuwGE...........................Pccz.................ScIyrisibirhhg#.kIR#w............bbqfrbibfghqGD...................",
".................yIuBz#....ztEpJ......................acccj....JpFLjzme......EzGInIE............................P#Vz..................vhrlibirhllUgXtpDzoIJzMIFGzFzG##xoabbqffhggWiI...................",
".................apvOFGY...HHnIV1.....................vGpIqqB1.GC#PVGxO.....ziqqDJK.............................kpvM..................rhibirhlbglgfUgoooyAooyomomtyDDyoDDoAyyfghlU0c...................",
".................opF.c#....JmnGv......................VxabxirkBptGNMcCuRkBuLrqqDArY.............................vM#H..................fbrpB#azAllqUg0roooyAoaaamiyyyoDDAaDx#HbllhWsy...................",
".................yIuBVH....#HQcv.....................eUgrHirlGFGH#KVGpGJJMcJ#bcCbbJ.............................wcvoO................uqqfyR#yCAqghU0sbyDiiAyriyAfabyriyAAahgffbsssgi...................",
".................ttMSzI....#aSzNY....................esgfLtrqbztIGJJzCtGczzIHfxDhlM.............................wcNz.................nllibAqrqqgqWgs0biriyyyArDryfiArDryrbsqUqqXg00oO..................",
".................CGcecF....VtEJN......................allbffgiCAxcvFGCmc#VGpbglrlhB.............................u#LG..................llrrrlqqgqWssg0bibirrbibfibirrbbhbbigX02llhWb#...................",
".................yIuBVx....Mmnvc......................GxHAsrUe.cxcMJ#In.....mhqsyMVY............................nzjp..................tghqlbrghlUWWX2bqfrqxgH#IIFVzFKzWWX0U2hUsX00oo...................",
".................yIFnzG....JmnzG......................LcjIsfR..jpGJJGxj......jIm#EFL............................RzuG..................uqhgffsllsl0sgLBIEGJ............H0WX02XU0s2aca...................",
".................#HNTcG....GtEFL.....................kDKVGk....Vt#NpztT.........IVkVO.....Y.....................QFPV..................Fc#bqssghU0s2aYBpk#J.............IhgXU0s22cu#a...................",
".................ytVScm....JmnzF.....................QCuGE.....GC#cJ#Hw.........So#v#.Y.........................QFPV.................vGvENfgsWUsX0IO.TFw#c...............lUsX0WavuzxY..................",
".................GpJSFH....JmnzF.....................DCM#d.....zHHKzIHS..........etEIV..........................EzNH................uJVQwIxgWX0UqL...dHw#F...............HggqfbaNjHt...................",
".................tHzkzH....MmPJK....................RaGzF......zH#FFcCS...........QxLIV.........................KIJm...............d#zww#HorMJuL......pw#V...............PfbabryJK#D...................",
".................zHzk#CO...JmPJt....................MoCpB......ztGFF#CSY..........R#pEpBY......................jlfbqr..............MFEEupDiF........eohfbAv..............MffbAbrNuGDd..................",
".................pHcjzC....MCPJV...................RamaN.......ztGzzFxL.............HMuG.......................rfbaoqc............EcukQIxAa.........olfbAblM.............RqrbabrFjGD...................",
".................FccPzC....wxjzH...................#DxyB.......LtGzzGpBY............EHwvV.....................JqrDJqbr............GzekcCabR........nhqrDrbqf.............PqfbqirpLpm...................",
".................zCcuFH....JacpDR.................Rtoab........MtGccctw..............PcBVQ.................RFHMyqrDriD.Y.........cVdnKHorQ.........jhfaSirls..............qrbirbqbqife.................",
".................GH#LVp...#shfbbau................uaooE........nxGzcVtw...............zVkc1.............PcCpQPuDqfff#FHQ........LcjRPpDAG.........kxhqbbrtAU..............qffirlfbxygbn................",
".................cCcjFcY.JshfbaDise..............dmDmD.........cCIJGFCP................FMFG1.........kKc#ukQJItGghqfpcVcjO......zN1Ectabd........pcGhfbbpHtG..............yrfAAirirlfhC................",
".................cCcMFHe.tshqaDirlM..............PaooV.........kxpHzcHQ................SIjJVd.....SKvFvwQvpHzL...jzPkxcVFHeY...zcekFHolS.......zpGFCtWshtpHt1.............xrfAirc#AAhfU................",
"................jAlAiyqiF0shlNPrysDvB............GaDAd.........LxGF#CGn.................uIQxlfHEjzVknwMIxVuY..........ztJVcPY.KFBBKHti#......RGzzGHmcdjHDCIVIO............IrfAirDtabhfUk...............",
"...............phfqrbiAfgUshqHabhhqHxGNR........nxaoo..........RDtzccpk..................plfbAbpLRnMICIww..............MIccFMjzPejGxyod....TJIzHGtFj....dmpG#u............IbririotaAhq0u...............",
"..............KshqfbryIabghbpGzbrUaCIxDfyMB....jHAmDN1.........jmIJG#pL..................CqrDriyN#pVk....................D#VF#VOQzColS....Mc#zcHtE.......RtG#CP...........NbqifiDtobWWs................",
"..............rWlfbryoriqfUitFHfglomDColrfApMBAUWlWa...........QmppHzmMd.................shrQrffxuYY......................JpcGOQvHtiL...kCIzHGoL..........jmpGxB..........jfyrbqaCDf0Uj................",
"..............UgqqbpLRxyglfbHzmgu.KcyoribabryAAhUlWld.........ChqqrbilAiR................zlAlAlskY.........................#zkeuGmafvnOzHGoIxV.............cmIpp.........JafbAbAACoqhJ.................",
".............uWslffomIafbbsDDCDd.....BGAffbiqyrffhfUJ........Igqqbrqrbilqe...............Rbtobst..........................TVV.LICabhhyCIxtApE..............RDHcCV.......jomfbiiriooL...................",
".............kUghblfbqD#abhomIt.........nVyrfbqifshXG.......Lgqqbbryfbilfa................HzFxG...........................vVdLJmxrirtApxDiw.................RttG#u.....pmmDfbiiiDtAR...................",
"..............rWgqqbbotzCCDotHzY...........kxhfUshXUG......RrfhfbbaPJqbqfUP..............d#upO...........................KFEjN#xriiVAfoaGd...................zIczC1..TFHoDirfiqyfma....................",
"..............#Wshqshtp#zGxyotF.............R20UsXUsjO.....MWWqfbrHQNbbqlWC..............PcMJY..........................RcVdLtCorqrVirsD......................HcCxyotzHtammqffyrfoc1...................",
"...............AUgWslomIGccttaw..............yXXX0smp......zhqfhqaccrbqqrgmvQ............cFMv...........................zceKFHmrbqqqrghjO......................DirqbpxDHorFfbAbArfv....................",
"................p0UgWfoxG#zGCod...............MflWaGoEY....zUlshlfyrfbilgqIcz#FwO.......eGN#S..........................VVQQVpmftEsDDqffe.......................bWfbatGxAANulffAAhUL....................",
".................d##shqmmGzzGHd..................SbmDfS....p0shlflAlsqfUht.kLJcctMES....jFvc..........................TcuwMICyAd.KcvDI.........................hqqDmtmDAPeUgiririqs#...................",
"....................IfyrDtz#VGp...................#i#DiS..EfUgWhqfhlWqslWCS..OBLVzHzzLe.KMFG.........................T#VQVcCoad..NFvc.........................TshljJmmqsulUgqrbilqWsu..................",
"....................TqrbiAHGF#cVd..................CrpmIOJrfh0UgshXfglhq0H#INRSZZTuVVMFVFvFP.........................VFLJcpmiIO..CJFM.........................whfbbalfbUG0sXUgWgUshUa..................",
".....................AArboCCIFF#v..................RAmCoHAiylUgWgshlshXUhotHCIzVkReekQJjuVVukZ......................uVVNVpHit...SIJGk..........................fblfflssht20UsXDishUsXd.................",
".....................KfbiiixpHzVpL..................waCaCaryrW0sXWsl0sXWWffiDDtCIzVNuBkPKJLuuJKB1..................uIFvc#DxfR...kCzC1.........................SGxhqqgsWuD0U0sXbqUgWgsL.................",
".....................xqrbiAyo#VGJGF..................CotyAirirs20UXU0Ugo.TExpbaDaCaGcGNjPuKFMjPPKVPTO..............F#NFcpohL....jHzC..........................VFdHshUfJ.u0Ug0Ug0UgWh2u.................",
".....................UsbryfyAtp#VF#P................HmmaCbribriJrgsW2Dv......RNtairmDCHzIcujN#MIKjjjJKB.......LxqffGzVFHaaG.....M#FG.........................QGnKCouuk..dXU0Ug0UUgWhg..................",
"....................KXXbqyoyAiHccVc#P.............dHomoribiqyHd..TO..............wJryrDDCppcGNKvF#zFukvFMjZZOAffqrghIzHtarR.....##pV.........................CPkcad.....diqXU0UUgW0UIO.................",
"....................mWlWfbiilAiHGzVF#S...........voaDimafAArM.......................KCqAaomtHHzIVuVF#zF#zvVNbbfblbqifixAAR......p#Cv.......................dzFTVoE......zhrbqXU0XU0h...................",
"....................KXlfbqrbggiDt#FGVpE........eGmmammaAifbN#T......................ubrUbqifixmDCppzFJVczcczfbiiibqifslAG.......tcpw.......................JpBFxp......jfbxHAUgW20G1...................",
"....................TWshfbbhq0iDttpJFFHdY.....dtmotyaiqyfppwvVd....................zlfbafNOwCryrDxpCppGcGVpHsrqNKyqhfqshV......R#FHn......................d#PQIaS.....BAAr#AArMFvd.....................",
".....................GUUgsWhfMFarxG#FF#tHyo#vJmxaooyrfbxBdH#PVuYd................BLiqyfpffe...kHAryomoHt#FGoqhyNPorlfgUsh......M#Fx.......................JFOGmB......oricCobQ.........................",
"......................JqWshh#djIzrmIppirlfiriomaiomabqx...RtvnAlqmd......dkPJVGmzcczbaRAffuYY....wItbrtmmGtobffiD#AlqhqUgY.....zVpm......................vVeuxp......joApHibV..........................",
".......................euVVmcJ.VpJyotgqqbboyAtyCAffqaQ.....IHorfyrfEujcVc#cpcVGF#cJVrboffiu.........R#yAiyAhqfhlaDAlWqWs2......tcIV.....................kzEuCpd.....drftGirte..........................",
"...........................QmHQeVpHyhqfhAloaDaDAAafxR.......ghblxbqDvVKJJG#pHmCIxtCDffhqcVM............dKHlfflfblblWqWsh2k.....p#Hv.....................#LTIaQ......DDacyAaR...........................",
"............................IxpTRcvolgbbabtaiarirlM........jhfbSmafAcVc#cpmiDizFLwRuWgqqGzzGTO............lUhfbbhlfghfUsi.....jD#CL....................MFOFxjO.....JafGmyrJ............................",
".............................zIzdLcshhfbbHMmmaAfbB.........LgfbxbrqqxAAGFEjw........eGl#pHVVMn............NWsl0hfUhfU0UXx....MlssrB...................uIBLIx......RraDGmyt.............................",
"..............................AGv.pUUgqrDFHAAhbql..........dshAmCbq#B...................BH#IJzE...........e0sXbWWqWshXX0k...zhfqfbi...................#KnGmR.....dtyCIyrfO.............................",
"..............................QotQQWWqWfbaffbhUUr...........tgtGcyoY.....................RFHccGF...........vXX0ghUgW2UgI...SWfhfalfJ.................pvBFtQ......oyotmDfE..............................",
"...............................zIzFWshhqbblfbqXXIO..........exDzVmB........................FHcctMY..........R0U00U000Uy...SushrLfxmIKO..............EHTKIc1.....wrmmmaAt...............................",
"................................HmGUshqshfbgsWhXnO............xc#z..........................KHzIGzO..........ex0Ug0UUgskQJGFfblbltp#Vppuk..........R#QQIaO......bqDpiAme...............................",
"................................dCpAUUgWhggsWhXf..............yzFx...........................kxGzcvn...........dJsshqirmFuLKyhshqmH#IGzVIIzLT...zAlfFTVou......MoyCAabR................................",
".................................BDtshUsh2hfU0WfnO............IGzt............................EpGF#ce............AArryrDRJHaaWshhqryoCIx#VG#pFFAffqaAapc......MrboCor#.................................",
"..................................JmxfU0U0000Ulfa.............HtHH.............................ExpvVNS..O......RNAlrfiffxaCuk#sXXbOBEpaCaGpcGcClfbAxAqr1.....SmmapDAod.................................",
"..................................RFH#pashUWsiriqL............oxyo..............................BIcz#pDAocn.SKMVVQqrbiAyxd.....ee......jzcDICIffiD#Affqu.....zAiDtAbR..................................",
"...................................TxG#SMIKbaDaDir............IxCod..............................dHGFffbAbA#FKnTPJbribbq#.................RFaoqrbinolrgM....JaftaAiF...................................",
"....................................QD#Mn#VGbaDaDrFY..........IHmD................................TIggibtrfETPN#xDirqirird...................BDgaCaohqfL...QoitmafH....................................",
".....................................QD#PzHGoAaoDqiR..........tDtCe................................jhfgFxqfmppDzk.KfrfifrQ....................nlpcGmfshwO..ryAxDAAd....................................",
"......................................FtFJVczfoxDirp..........ACIxB.................................UsbabhhloEd....xbiriqH.....................DIIpDshDddeJAaoDirJ.....................................",
"......................................QDItvItmitmiriE.........cCcGdY................................hCHahfUf.......zbabrybd....................CppGhU2mWXXsirtirH......................................",
".......................................nmcHzirlfmDAiDe........MzccE.................................CppDshUuYY.....wrforfiR...................QIzItvLf200UXloAafO......................................",
"........................................QDxrhfqfoqAafM........tyAiF................................KCpDAUsI.........Ibqifi#...................#zGxxeXXs0Ug0UUbqSZ......................................",
"........................................nmfblblbyAiyAad......MlbqyqfO.............................EGzzycPT..........Mbabrya..................TIGztGcXX0g0sXWWWr........................................",
".........................................ghblAhhfbbaiDrcDIFjJyAfoDfrp............................Rpc#pH.............RbryrDbQ.................JIczoErfh0WWqWUsXWe.......................................",
"........................................BgqqbariirioryrDbabCIxGxxxrig............................CpFHc...............xqAffquY...............dCIxyAaioDf0foh0Ul0J.......................................",
"........................................RWsl0hAforfifryrfoAmCItpOolrgNPS........................JVczoR...............virirAid.............RuJiDromaarxrgsfgUsXWx.......................................",
"........................................Ssh2hlAiiriooirmrbqamatobfbq#LvVNuk....................QpG#CV.................byAiylAid.......EuzCoyAiyAariiarihUUg0sXWG.......................................",
".........................................Al0UlqfbrbqyqyAafGmyfbbfblbxn.SPPFFJk................RpVFHG..................HbryfyqshL.BkjzyAiyAaotyaomoaooybqUsXWshUuYY.....................................",
".........................................ElssXXirioririootGxgsWhhfUhmppMu1TKFF#cj1............ccVGDR.................NWrfifrygUsCorDDaDmtoaDayoyryrbrqqUgW2UW0l........................................",
"..........................................RWXX0gbrqqyrfbAAhUUgW0shUIQ#Atp#VLekPN#IINu1......jzc#ztw..................bUlAforbrUshyAtyaDAAaiorfifrygbqU0UXUsXWsTO.......................................",
"............................................QLvOwoqArfAiryrfUgW2Axe....RMtmCHzFTBwMPKVue.dDgrfixpv..................wUUgqabAhhWshhomaaiDrobryfyqyorfApIhUXX0mw.........................................",
"..................................................HfbAbAryrfbWsl...........IGxpHccESBwPuVfbiiibh#Y..................FggUlsyqhUgW0sAiylArfAiryAafirsInO..uLuk...........................................",
"..................................................TGhqqDrbqqq0U0d.............vpHtHzFQB1Qqbbyoyrt...................Cr200sDDqUgW0sAryrbrfhfMiDrrfhE....................................................",
"....................................................UUgqlfb0Ug0UV................RNAxGzGpqrDeyorse...............BzomWshXWshXWWs2WryAtGmgsfgqqbbWJ.....................................................",
"....................................................UUgWgyAshUslomoLT...............BVpmbqifJmxGxS.............vxpCpiqgqqWshXWWshqpG#LuGhshlshq02u.....................................................",
"....................................................U0Ug0UUgUgWhmMuMFVQRe...............AshhrmGcGpM1.........jopcGcCoWshhqUgWgsXXbukuGmxhq0ghUUsbY.....................................................",
"....................................................lU0U0000shXCEdZZwQuLccS.............ohfbbaxGHzIGM......MoIzItpxDCfWWqWfshUsXXFcCDooagUsh0U00I......................................................",
"....................................................G0UgqlAlsqWH#FvdO.TkPzHtKnwO.........AUgWhrmmppzItE..JHGztHGxpCDmrq0Ug0UUgU0lxDoaDpBYG0UUgqIO......................................................",
"....................................................elUhshhshXfyCItG#VkRYekv#pFJVn.......LAUWWatApxIczoCppGHt#xpoaDaqrDr0Ug0UUXqxirtQR....dVpLk........................................................",
".....................................................KUsXWsUUgejmmamxpH#zvRd.wMJFF#PjO.....KsWHAvrmmppG#CIIptpxyAiyAIINkvhUXUgql#P.....................................................................",
"......................................................eGUsXUG.....dGmyttHHGFJkSnEMJ#pHVEdZNAAhUUsAtGcHt#xIxCDDabqx#vBkv#mDAiAvES.......................................................................",
".........................................................Tk..........BKDmrtHHGcMLwEjNVFHGxbifyrDaGppppppGxmDfbrUbnkMGmytyrJN1..........................................................................",
".........................................................................EtaAmIppGzzjNuJEFqffyocHt#xIHmCariibqlbryCamafALT.............................................................................",
"............................................................................RVFAapCHzcMLuDqirKFacpcGmxDAiribribghbomaJk................................................................................",
"...............................................................................dKIAxmmGtzhbqiLJrmmpaooybfbbfbshhssxB...................................................................................",
"..................................................................................dKcpaCxhfbbH#bryabhhllfghqfU2XX0.....................................................................................",
"......................................................................................P#slfbqffifbAgqqbbWfbh2220sm.....................................................................................",
"........................................................................................yhslfblfghq2IJQ.dg220XXs0......................................................................................",
"........................................................................................KXlggUqslWWIO....dhU2UUrO......................................................................................",
".........................................................................................GhWlWfgsW2O.......VpFk........................................................................................",
".........................................................................................ThUXUshXfk....................................................................................................",
"..........................................................................................RtWsXXxB.....................................................................................................",
".............................................................................................Eue.......................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
".......................................................................................................................................................................................................",
"......................................................................................................................................................................................................."};


/*
 *  Constructs a AboutDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AboutDialog::AboutDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      image0( (const char **) image0_data )
{
    if ( !name )
	setName( "AboutDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 228, 370 ) );
    setMaximumSize( QSize( 228, 370 ) );
    setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 245, 244, 235) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 254, 249) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 254, 249) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    setPalette( pal );

    PixmapLabel1 = new QLabel( this, "PixmapLabel1" );
    PixmapLabel1->setGeometry( QRect( -2, -7, 227, 196 ) );
    PixmapLabel1->setFocusPolicy( QLabel::StrongFocus );
    PixmapLabel1->setPixmap( image0 );
    PixmapLabel1->setScaledContents( FALSE );
    PixmapLabel1->setAlignment( int( QLabel::AlignCenter ) );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( -3, 261, 232, 71 ) );
    TextLabel1->setTextFormat( QLabel::AutoText );
    TextLabel1->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setGeometry( QRect( 7, 338, 213, 28 ) );
    PushButton1->setDefault( TRUE );

    qt_version_label = new QLabel( this, "qt_version_label" );
    qt_version_label->setGeometry( QRect( -2, 240, 232, 16 ) );
    qt_version_label->setTextFormat( QLabel::AutoText );
    qt_version_label->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    BALL_version_label = new QLabel( this, "BALL_version_label" );
    BALL_version_label->setGeometry( QRect( -1, 219, 232, 16 ) );
    BALL_version_label->setTextFormat( QLabel::AutoText );
    BALL_version_label->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    BALLView_version_label = new QLabel( this, "BALLView_version_label" );
    BALLView_version_label->setGeometry( QRect( -2, 189, 232, 27 ) );
    BALLView_version_label->setTextFormat( QLabel::AutoText );
    BALLView_version_label->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    languageChange();
    resize( QSize(228, 370).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( PushButton1, SIGNAL( pressed() ), this, SLOT( close() ) );

    // tab order
    setTabOrder( PixmapLabel1, PushButton1 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
AboutDialog::~AboutDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AboutDialog::languageChange()
{
    setCaption( tr( "About BALLView" ) );
    TextLabel1->setText( tr( "Andreas Moll,\n"
"Andreas Hildebrandt, \n"
"Heiko Klein, \n"
"Oliver Kohlbacher" ) );
    PushButton1->setText( tr( "O&K" ) );
    PushButton1->setAccel( QKeySequence( tr( "Alt+K" ) ) );
    qt_version_label->setText( tr( "QT Version" ) );
    BALL_version_label->setText( tr( "(BALL 1.0)" ) );
    BALLView_version_label->setText( tr( "<h2><b>BALLView 1.0</b></h2>" ) );
}

