

void GetNameTransformation(int number, byte mode = 0)
{
	// première fois que je créer une fonction surchargée !
	// la plupart du temps elle sera appelée sans le mode 1, qui rajoute un chiffre au début
	//
	// TODO DEBUG c'est moche, ça place le résultat dans la variable NameTransfo
	// j'aimerais mieux que ça retourne un String !
	// pour le quick JUMP , voir SubParts.h
	// void checkButton6LayerSelect ( void ) {
	// byte jump[]={111,244,228,218,190,167,81,127,64,216,108,111} ;
	//
	switch (number)
	{
	case 0:		NameTransfo = F("zero");		break;
	case 1:		NameTransfo = F("Continuous Random Melody");		break;
	case 2:		NameTransfo = F("Hammer");		break;
	case 3:		NameTransfo = F("Hammer Funk");		break;
	case 4:		NameTransfo = F("Transpose");		break;
	case 5:		NameTransfo = F("Delete Layer");		break;
	case 6:		NameTransfo = F("Create 60");		break;
	case 7:		NameTransfo = F("Copy+CH");		break;
	case 8:		NameTransfo = F("EquaLgth");		break;
	case 9:		NameTransfo = F("Deplace");		break;
	case 10:		NameTransfo = F("Decimate");		break;
	case 11:		NameTransfo = F("multiply");		break;
	case 12:		NameTransfo = F("MELtiply");		break;
	case 13:		NameTransfo = F("CH Change");		break;
	case 14:		NameTransfo = F("CH Info");		break;
			case 15:		NameTransfo = F("MkrPoly");		break;
	case 16:		NameTransfo = F("Populate");		break;
	case 17:		NameTransfo = F("Flip Loc");		break;
	case 18:		NameTransfo = F("GoldRatio");		break;
	case 19:		NameTransfo = F("Sketch");		break;
	case 20:		NameTransfo = F("(un)Mute");		break;
	case 21:		NameTransfo = F("Reverse");		break;
	case 22:		NameTransfo = F("PFunk");		break;
	case 23:		NameTransfo = F("AutoFX");		break;
	case 24:		NameTransfo = F("RECORD");		break;
	case 25:		NameTransfo = F("MidiOX ");		break;
	case 26:		NameTransfo = F("Speed ");		break;
	case 27:		NameTransfo = F("Copy-CH ");		break;
	case 28:		NameTransfo = F("Add+CH ");		break;
	case 29:		NameTransfo = F("Add-CH ");		break;
	case 30:		NameTransfo = F("Eye Candy ");		break;
	case 31:		NameTransfo = F("Layer Choose ");		break;
	case 32:		NameTransfo = F("First Free Note ");		break;
	case 33:		NameTransfo = F("Schizo ");		break;
	case 34:		NameTransfo = F("Schizo2 ");		break;
	case 35:		NameTransfo = F("miRRor ");		break;
	case 36:		NameTransfo = F("reflet_ ");		break;
	case 37:		NameTransfo = F("ShrExp-p ");		break;
	case 38:		NameTransfo = F("reVert ");		break;
	case 39:		NameTransfo = F("Velocity ");		break;
	case 40:		NameTransfo = F("Harmonize ");		break;
	case 41:		NameTransfo = F("Root Note ");		break;
	case 42:		NameTransfo = F("REC mode ");		break;
	case 43:		NameTransfo = F("Ping Pong ");		break;
	case 44:		NameTransfo = F("Morse ");		break;
	case 45:		NameTransfo = F("Var Speed ");		break;
	case 46:		NameTransfo = F("CH RA info ");		break;
	case 47:		NameTransfo = F("Fidelity ");		break;
	case 48:		NameTransfo = F("CH FI info ");		break;
	case 49:		NameTransfo = F("Flip Not ");		break;
	case 50:		NameTransfo = F("Tabula Rasa ");		break;
	case 51:		NameTransfo = F("Thru WER ");		break;
	case 52:		NameTransfo = F("ND4 Horse ");		break;
	case 53:		NameTransfo = F("Slicer H ");		break;
	case 54:		NameTransfo = F("Scramble ");		break;
	case 55:		NameTransfo = F("BassLine ");		break;
	case 56:		NameTransfo = F("Concat100 ");		break;
	case 57:		NameTransfo = F("Excat 100 ");		break;
	case 58:		NameTransfo = F("Funky Vel ");		break;
	case 59:		NameTransfo = F("LG Not ");		break;
	case 60:		NameTransfo = F("Wobbly Delay ");		break;
	case 61:		NameTransfo = F("EcarTele ");		break;
	case 62:		NameTransfo = F("Ins CC Rnd ");		break;
	case 63:		NameTransfo = F("REP Fract ");		break;
	case 64:		NameTransfo = F("Ladder ");		break;
	case 65:		NameTransfo = F(">StartNote ");		break;
	case 66:		NameTransfo = F(">NbSteps ");		break;
	case 67:		NameTransfo = F(">LG Note ");		break;
	case 68:		NameTransfo = F(">LG Spce ");		break;
	case 69:		NameTransfo = F(">Inc LG ");		break;
	case 70:		NameTransfo = F(">Inc Spce ");		break;
	case 71:		NameTransfo = F(">Step Size ");		break;
	case 72:		NameTransfo = F(">Inc Step ");		break;
	case 73:		NameTransfo = F("--- ");		break;
	case 74:		NameTransfo = F("SortPitch ");		break;
	case 75:		NameTransfo = F("TX81Z Random ");		break;
	case 76:		NameTransfo = F("Split n");		break;
	case 77:		NameTransfo = F("RndUp  ");		break;
	case 78:		NameTransfo = F(">RndUpScope ");		break;
	case 79:		NameTransfo = F("Deplace% ");		break;
	case 80:		NameTransfo = F(">DeplaceScope ");		break;
	case 81:		NameTransfo = F("SAVE layers ");		break;
	case 82:		NameTransfo = F("LOAD layers ");		break;
	case 83:		NameTransfo = F("DaubyMod ");		break;
	case 84:		NameTransfo = F("PLAY midi ");		break;
	case 85:		NameTransfo = F("TX81Z Setup ");		break;
	case 86:		NameTransfo = F(">MIDIlong ");		break;
	case 87:		NameTransfo = F(">LD Fgt ");		break;
	case 88:		NameTransfo = F(">LD NextFgt ");		break;
	case 89:		NameTransfo = F(">LD PrevFgt ");
		break;
	case 90:		NameTransfo = F(">LD +1Fgt ");		break;
	case 91:		NameTransfo = F(">LD -1Fgt ");		break;
	case 92:		NameTransfo = F(">LD RND Fgt ");break;
	case 93:		NameTransfo = F(">MIDIFile ");break;
	case 94:		NameTransfo = F("TapSqueeze");break;
	case 95:		NameTransfo = F("TapKeepLgth");break;
	case 96:		NameTransfo = F("Grid");	break;
	case 97:		NameTransfo = F("Euclidian");		break;
	case 98:		NameTransfo = F(">Note ");		break;
	case 99:		NameTransfo = F(">k pulse");		break;
	case 100:		NameTransfo = F(">tot size");		break;
	case 101:		NameTransfo = F(">LG");		break;
	case 102:		NameTransfo = F(">Shift");		break;
	case 103:		NameTransfo = F(">Vel");		break;
	case 104:		NameTransfo = F("Snd2CH SMP");		break;
	case 105:		NameTransfo = F("Mut CH Not");		break;
	case 106:		NameTransfo = F("GATE Mult");		break;
	case 107:		NameTransfo = F(">GATE Nb");		break;
	case 108:		NameTransfo = F("Pitch MAPPING");		break;
	case 109:		NameTransfo = F(">Max VMap Tgt");		break;
	case 110:		NameTransfo = F(">Min VMap Tgt");		break;
	case 111:		NameTransfo = F("message");		break;
	case 112:		NameTransfo = F("AutoHarmonize");		break;
	case 113:		NameTransfo = F("multipSqueeze");		break;
	case 114:		NameTransfo = F("Clean UP");		break;
	case 115:		NameTransfo = F("Paral.lel Midi PLAY");		break;
	case 116:		NameTransfo = F("SYMETRIC");		break;
	case 117:		NameTransfo = F("ShrExp Abs");		break;
	case 118:		NameTransfo = F("BPM");		break;
	case 119:		NameTransfo = F("Phasage k/-");		break;
	case 120:		NameTransfo = F(">SET -/n Phasage");		break;
	case 121:		NameTransfo = F("Rotate in degrees");		break;
	case 122:		NameTransfo = F("> Set x (longueur layer)");		break;
	case 123:		NameTransfo = F("> Set y (pitch)");		break;
	case 124:		NameTransfo = F("Affichage (x,y)");		break;
	case 125:		NameTransfo = F("Activate EVENT");		break;
	case 126:		NameTransfo = F("Select CC/Transfo");		break;
	case 127:		NameTransfo = F(">EVT Select");		break;
	case 128:		NameTransfo = F(">Delete EVENT");		break;
	case 129:		NameTransfo = F(">SET Type of Event");		break;
	case 130:		NameTransfo = F(">SET Intervalle");		break;
	case 131:		NameTransfo = F(">SET Moment dans l'intervalle");		break;
	case 132:		NameTransfo = F(">SET Fidelite");		break;
	case 133:		NameTransfo = F(">SET Type d'intervalle");		break;
	case 134:		NameTransfo = F(">Select Value to send to CC");		break;
	case 135:		NameTransfo = F(">SET Shape of value change");		break;
	case 136:		NameTransfo = F(">SET RangeUP");		break;
	case 137:		NameTransfo = F(">SET RangeLO");		break;
	case 138:		NameTransfo = F(">SET Increment");		break;
	case 139:		NameTransfo = F(">SET BehaviorLimit");		break;
	case 140:		NameTransfo = F(">SET Channel");		break;
	case 141:		NameTransfo = F(">SET Layers");		break;
	case 142:		NameTransfo = F(">iVLVDelta Midi Glitch [Nl=0]");		break;
	case 143:		NameTransfo = F(">iDureeDelta Midi Glitch [Nl=0]");		break;
	case 144:		NameTransfo = F(">imididouble midi Glitch [Nl=0]");		break;
	case 145:		NameTransfo = F(">iShiftDelta midi Glitch [Nl=8]");		break;
	case 146:		NameTransfo = F(">iShiftDuree midi Glitch [Nl=7]");		break;
	case 147:		NameTransfo = F(">iDureeMask midi Glitch [Nl=127]");		break;
	case 148:		NameTransfo = F(">iMask2 midi Glitch [Nl=128]");		break;
	case 149:		NameTransfo = F(">CutOffDureeDelta midi Glitch [Nl=x]");		break;
	case 150:		NameTransfo = F(">BringToDureeDelta midi Glitch [Nl=x]");		break;
	case 151:		NameTransfo = F(">NUM Ratio VLV2MSEC midi Glitch");		break;
	case 152:		NameTransfo = F(">DENOM Ratio VLV2MSEC midi Glitch");	break;
	case 153:		NameTransfo = F("Create Mask");		break;
	case 154:		NameTransfo = F("Swing");		break;
	case 155:		NameTransfo = F("> n Swing");		break;
	case 156:		NameTransfo = F("> pattern Swing");		break;
	case 157:		NameTransfo = F("Fuse");		break;
	case 158:		NameTransfo = F(">Type Fuse");		break;
	case 159:		NameTransfo = F("Custom Swing Pattern");		break;
	case 160:		NameTransfo = F("Src MidiMatrix redirect");		break;
	case 161:		NameTransfo = F(">Tgt/isolated MidiMatrix redirect");		break;
	case 162:		NameTransfo = F(">Special MidiMatrix fc");		break;
	case 163:		NameTransfo = F(">MidiMatrix");		break; // affiche la ligne actuelle de redirection
	case 164:		NameTransfo = F("SkipChunk");		break;
	case 165:		NameTransfo = F("LG note min-max Rnd");		break;
	case 166:		NameTransfo = F(">LG note max");		break;
	case 167:		NameTransfo = F("Reset MIDI Parallel Original values");		break;
	case 168:		NameTransfo = F("Static LO cut");		break;
	case 169:		NameTransfo = F("Static HI cut");		break;
	case 170:		NameTransfo = F("Dynamic LO cut");		break;
	case 171:		NameTransfo = F("Dynamic HI cut");		break;
	case 172:		NameTransfo = F("TX81Z Send CC");		break;
	case 173:		NameTransfo = F("Cut --] end");		break;
	case 174:		NameTransfo = F("Cut [-- start");		break;
	case 175:		NameTransfo = F(">MAX Tgt Horiz MAP");		break;
	case 176:		NameTransfo = F(">min Tgt Horiz MAP");		break;
	case 177:		NameTransfo = F(">MAX Src Horiz MAP");		break;
	case 178:		NameTransfo = F(">min Src Horiz MAP");		break;
	case 179:		NameTransfo = F("Horizontal Map");		break;
	case 180:		NameTransfo = F("Breathe");		break;
	case 181:		NameTransfo = F(">Type breathe");		break;
	case 182:		NameTransfo = F("Replicate");		break;
			case 183:		NameTransfo = F("Disjoint");		break;
	case 184:		NameTransfo = F("SAVE all events");		break;
	case 185:		NameTransfo = F(">Note Fregoli Cotard");		break;
	case 186:		NameTransfo = F("Fregolize");		break;
	case 187:		NameTransfo = F("Cotardize");		break;
	case 188:		NameTransfo = F("Macro Frego/Cotard");		break;
	case 189:		NameTransfo = F(">Source CCMod");		break;
	case 190:		NameTransfo = F("CCMod SET Current");		break;
	case 191:		NameTransfo = F(">SET MinSource");		break;
	case 192:		NameTransfo = F(">SET MaxSource");		break;
	case 193:		NameTransfo = F(">SET MinTarget");		break;
	case 194:		NameTransfo = F(">SET MaxTarget");		break;
	case 195:		NameTransfo = F(">SET Border Behavior");		break;
	case 196:		NameTransfo = F(">SET CC Nb");		break;
	case 197:		NameTransfo = F(">SET CC Chanel");		break;
	case 198:		NameTransfo = F(">SET Layers concernes");		break;
	case 199:		NameTransfo = F(">SET //  Midi CCMod");		break;
	case 200:		NameTransfo = F(">Activate CCMod");		break;
	case 201:		NameTransfo = F("MIDI Thru");		break;
	case 202:		NameTransfo = F("Play note");		break;
	case 203:		NameTransfo = F(">Select Channel to play");		break;
	case 204:		NameTransfo = F("next play ");		break;
	case 205:		NameTransfo = F(">DureePole1 ");		break;
	case 206:		NameTransfo = F(">minPole1 ");		break;
	case 207:		NameTransfo = F(">maxPole1 ");		break;
	case 208:		NameTransfo = F(">DureePole2 ");		break;
	case 209:		NameTransfo = F(">minPole2 ");		break;
	case 210:		NameTransfo = F(">maxPole2 ");		break;
	case 211:		NameTransfo = F(">FreqPole1 0..100 ");		break;
	case 212:		NameTransfo = F("BipolarRandom ");		break;
	case 213:		NameTransfo = F("Map Velocite");		break;
	case 214:		NameTransfo = F(">Vel Min Target ");		break;
	case 215:		NameTransfo = F(">Vel Max Target ");		break;
	case 216:		NameTransfo = F("Pouf (time funk)");		break;
	case 217:		NameTransfo = F("Kill note");		break;
	case 218:		NameTransfo = F("Cell");		break;
	case 219:		NameTransfo = F(">CH for Cell");		break;
	case 220:		NameTransfo = F("SoftPass");
		break;
	case 221:		NameTransfo = F("Cell Delete ALL");		break;
	case 222:		NameTransfo = F("Fit Euclidian");		break;
	// case 223:		NameTransfo = F("Sentence REC");		break;  DISMISS 2C22 
	case 224:		NameTransfo = F("Adjust --] end");		break;
	case 225:		NameTransfo = F("Adjust [-- start");		break;
	case 226:		NameTransfo = F("Modify SEQ");		break;
	case 227:		NameTransfo = F("Delete SEQ");		break;
	case 228:		NameTransfo = F("SEQ Select");		break;
	case 229:		NameTransfo = F("Erase(1) SEQ");		break;
	case 230:		NameTransfo = F(">Set Type SEQ");		break;
	case 231:		NameTransfo = F(">MinUP");		break;
	case 232:		NameTransfo = F(">MaxUP");		break;
	case 233:		NameTransfo = F(">MinDOWN");		break;
	case 234:		NameTransfo = F(">MaxDOWN");		break;
	case 235:		NameTransfo = F(">StaticMin");		break;
	case 236:		NameTransfo = F(">StaticMax");		break;
	case 237:		NameTransfo = F(">RndFrag Size");		break;
	case 238:		NameTransfo = F(">RndFrag Repeats");		break;
	case 239:		NameTransfo = F(">Set Action");		break;
	case 240:		NameTransfo = F(">Set CC sent by Seq");		break;
	case 241:		NameTransfo = F(">Set channel for CC sent by Seq");		break;
	case 242:		NameTransfo = F(">Set layer for seq");		break;
	case 243:		NameTransfo = F(">Activate Sequence");		break;
	case 244:		NameTransfo = F("DynHarmonize");		break;
	case 245:		NameTransfo = F(">Mode DynHarmonize");		break;
	case 246:		NameTransfo = F("Counters");		break;
	case 247:		NameTransfo = F(">SEQ Counter");		break;
	case 248:		NameTransfo = F("Eclipse Layer");		break;
	case 249:		NameTransfo = F("DISMISSED Markov GLB");		break;
	case 250:		NameTransfo = F("Tap Accou");		break;
	case 251:		NameTransfo = F("Mode (no harmonize)");		break;
	case 252:		NameTransfo = F("AutoCleanUP");		break;
	// 1C80 Regression
	case 253:		NameTransfo = F("Hash");		break;
	case 254:		NameTransfo = F(">%repeats ");		break;
	case 255:		NameTransfo = F(">repeatsNb");		break;
	case 256:		NameTransfo = F(">%permute");		break;
	case 257:		NameTransfo = F(">%inverter");		break;
	case 258:		NameTransfo = F(">unify");		break;
	case 259:		NameTransfo = F("ReUnited");		break;
	case 260:		NameTransfo = F(">DeleteCCMod");		break;

	// 1C81Regression2
	case 261:		NameTransfo = F("Activate Condition");		break;
	case 262:		NameTransfo = F(">WhichTest");		break;
	case 263:		NameTransfo = F(">WhichOperation");		break;
	case 264:		NameTransfo = F(">WhichValue");		break;
	// case 265 : NameTransfo = F(">WhichOperation") ; break ;
	case 265:		NameTransfo = F(">WhichComparator");		break;
	case 266:		NameTransfo = F(">WhichReference");		break;
	case 267:		NameTransfo = F(">WhichAction");		break;
	case 268:		NameTransfo = F(">TestValues");		break;
	case 269:		NameTransfo = F(">WhichLayer2test");		break;
	case 270:		NameTransfo = F(">ModuloLayer");		break;
	case 271:		NameTransfo = F("SpanDex");		break;
	case 272:		NameTransfo = F("SpanFlex");		break;
	case 273:		NameTransfo = F("KeepSyncopBUG");		break;
	case 274:		NameTransfo = F(">Replicate Evt");		break;
	case 275:		NameTransfo = F(">Suspend All Evt");		break;
	case 276:		NameTransfo = F("PANIC");		break;
	case 277:		NameTransfo = F("mrrorExtended");		break;

		// Set Compteur d'Event
		// Set latence Event
		// Set Echeance Event
		// Set Echeance ON

	case 278:		NameTransfo = F(">SetCompteur");		break;
	case 279:		NameTransfo = F(">SetLatence");		break;
	case 280:		NameTransfo = F(">SetEcheance");		break;
	case 281:		NameTransfo = F(">SwitchEcheance");		break;
	case 282:		NameTransfo = F(">Load ALL events");		break;
	case 283:		NameTransfo = F("Whole L fidelity");		break;
	case 284:		NameTransfo = F("Numerote Transfo");		break;
	case 285:		NameTransfo = F(">Seq Save ALL");		break;
	case 286:		NameTransfo = F(">Seq Load ALL");		break;
	case 287:		NameTransfo = F(">CCMod Save ALL");		break;
	case 288:		NameTransfo = F(">CCMod Load ALL");		break;
	case 289:		NameTransfo = F(">Cell Save ALL");		break;
	case 290:		NameTransfo = F(">Cell Load ALL");		break;
	case 291:		NameTransfo = F("Rename File");		break;
	case 292:		NameTransfo = F("Remove File");		break;
	case 293:		NameTransfo = F("Octaver");		break;
	case 294:		NameTransfo = F(">% Nt Octaver");		break;
	case 295:		NameTransfo = F(">Ratio vel Octaver");		break;
	case 296:		NameTransfo = F(">FunkyVel MIDI");		break;
	case 297:		NameTransfo = F(">ShiftR Seq");		break;
	case 298:		NameTransfo = F(">ShiftL Seq");		break;
	case 299:		NameTransfo = F(">Rndmize Seq");		break;
	case 300:		NameTransfo = F(">Permute Seq");		break;
	case 301:		NameTransfo = F(">Mirror Seq");		break;
	case 302:		NameTransfo = F(">RevertH Seq");		break;
	case 303:		NameTransfo = F(">Replicate Seq");		break;
	case 304:		NameTransfo = F(">Double Seq");		break;
	case 305:		NameTransfo = F(">Seq select");		break;
	case 306:		NameTransfo = F("Soft Mute (bypass switches)");		break;
	case 307:		NameTransfo = F("LED mode");		break;
	case 308:		NameTransfo = F(">Blink time");		break;
	case 309:		NameTransfo = F("Scalpel CH");		break;
	case 310:		NameTransfo = F("BPM sniffer");		break;
	case 311:		NameTransfo = F("Editha");		break;
	case 312:		NameTransfo = F("Axe12Tones");		break;
	case 313:		NameTransfo = F("RealScaleTranspose");		break;
	case 314:		NameTransfo = F("FixToRndClosestScaleMode");		break;
	case 315:		NameTransfo = F("RndAllTriadChange");		break;
	case 316:		NameTransfo = F("OneContRndTriadChange");		break;
	case 317:		NameTransfo = F("OneSpacedTriadChange");		break;
	case 318:		NameTransfo = F("UnFlattenTriads");		break;
	case 319:		NameTransfo = F("aroundC60");		break;
	case 320:		NameTransfo = F("RealModeTranspose");		break;
	case 321:		NameTransfo = F("SuperSymmetry");		break;
	case 322:		NameTransfo = F("Comb");		break;
	case 323:		NameTransfo = F("Comb2");		break;

	case 324:		NameTransfo = F("inspect");		break;
	case 325:		NameTransfo = F("delete note ");		break;
	case 326:		NameTransfo = F("Vel 2 Vol");		break;

	// nouvelles fonctions liées à la pile : le delay dynamique

	case 327:		NameTransfo = F("Delay Layer");		break;
	case 328:		NameTransfo = F(">nb delais");		break; // + SEQ DONE
	case 329:		NameTransfo = F(">space");		break; // + SEQ DONE
	case 330:		NameTransfo = F(">vel Drop");		break;
	case 331:		NameTransfo = F(">vel Down");		break; // +SEQ 2C19
	case 332:		NameTransfo = F(">rnd happn");		break;

case 333 : NameTransfo = F(">Repeater"); break ; // définit un nb de 16ème pour delay 
case 334 : NameTransfo = F(">BitMap"); break ; // definit un pattern on/off de répétition pour le delay 
case 335 : NameTransfo = F("Zat"); break ; // Drunk basé sur SEQ, par note 

	case 336:		NameTransfo = F("overflow");		break;
	case 337:		NameTransfo = F("max nb notes/layer");		break;
	case 338:		NameTransfo = F(">value Note");		break;
	case 339:		NameTransfo = F("Broken Euclidien");		break;
	case 340:		NameTransfo = F("SV L>Mem");		break;
	case 341:		NameTransfo = F("LD Mem>L");		break;
	case 342:		NameTransfo = F("MemSrc>Mem");		break;
	case 343:		NameTransfo = F(">set MemSrc");		break;
	case 344:		NameTransfo = F("SaveToUndo");		break;
	case 345:		NameTransfo = F("UNDO");		break;
	case 346:		NameTransfo = F("pitchableL");		break;
	case 347:		NameTransfo = F("pitchableCH");		break;
	case 348:		NameTransfo = F("eraseREC");		break;
	case 349:		NameTransfo = F("dynSwing");		break;
	case 350:		NameTransfo = F("REC Automation");		break;

		// nouvelles fonctions liées à barocco

	case 351:		NameTransfo = F("Barocco");		break;
	case 352:		NameTransfo = F(">seq");		break;
	case 353:		NameTransfo = F(">space");		break; // espacement +SEQ 
	case 354:		NameTransfo = F(">vel Drop");		break;
	case 355:		NameTransfo = F(">rnd happn");		break;
	case 356:		NameTransfo = F(">%SymH");		break; // indique le pourcentage de symétrie parfaite - inverse = asymétrie
	case 357:		NameTransfo = F(">%SymV");		break; // indique le pourcentage de réflexion - inverse = reflet
	case 358:		NameTransfo = F(">%Reflet");		break;

	case 359:		NameTransfo = F("PAN microSampler");		break; // ON OFF pour PAN sur channel et layer spécifique
	case 360:		NameTransfo = F(">size PAN");		break; // 0 = nul, 60 = maximum
	
	case 361:		NameTransfo = F("Pitch microSampler");		break; // ON OFF pour PAN sur channel et layer spécifique
	case 362:		NameTransfo = F(">size Ppitch");		break; // 0 = nul, 60 = maximum
	case 363:		NameTransfo = F("GEN Markov");		break;
		case 364 :		NameTransfo = F("LAY Markov");		break ;
		case 365 :		NameTransfo = F("F8 send");		break ; 
	case 366 :	NameTransfo = F(">PreReflet");	break ;
	case 367:		NameTransfo = F(">PreH");		break;
	case 368:		NameTransfo = F(">PreV");		break;

case 369 :NameTransfo = F("Derazer");break ;
case 370:	NameTransfo = F("CH Derazer r");	break; // ON OFF pour PAN sur channel et layer spécifique
case 371:	NameTransfo = F("Multiplier");	break;  
case 372 :NameTransfo = F("Save Local") ;break ; // SAuve le fichier en cours, sur le nom choisi ( DEFAULT.txt pour commencer )
// si on veut un autre nom, on utilisera fc "Rnd Name" ; ou "Save AS"
case 373 :NameTransfo = F("Load Local") ;break ; // Va effacer les mémoires et charger le contenu total 
case 374 :NameTransfo = F("FileName") ;break ; // va modifier le nom sous lequel on va sauver le fichier
case 375 : NameTransfo = F("Omit") ; break ; // va "oublier" le note OFF dans un % des cas 
case 376 : NameTransfo = F("freeMem") ; break ; // donne la mémoire libre sur Teensy.  DEBUG pour évaluer usage mémoire 
case 377 : NameTransfo = F("Generate") ; break ; // va générer un nom pour fileName ( éviter de devoir écrire )
case 378 : NameTransfo = F("Layer->Seq") ; break ; // va transformer la 1ère layer sélectionnée en Séquence
case 379 : NameTransfo = F("TAP REC IN"); break ; // débute un enregistrement 
case 380 : NameTransfo = F("Kompakter (ms)"); break ; // première version de compacteur : regroupe les notes distantes <n msec

case 381:	NameTransfo = F("Kannon");	break;
case 382:	NameTransfo = F(">Delay Kannon");	break;   // introduire SEQ !
case 383:	NameTransfo = F(">Pitch Kannon");	break;  // introduire SEQ !
case 384:	NameTransfo = F(">stretch Kannon");	break;
case 385:	NameTransfo = F(">revert");	break;
case 386:	NameTransfo = F(">reflet");	break; // indique le pourcentage de symétrie parfaite - inverse = asymétrie
case 387 : NameTransfo = F(">%Rnd Happen Kannon"); break ; // pourcentage de random d'apparition du kannon 

case 388 : NameTransfo = F("Kompakter (N)"); break ; // second compacteur, réduisant à N notes alignées 

case 389 : NameTransfo = F("LFO CC"); break ; // LFO ON/OFF
case 390 : NameTransfo = F(">type d'onde"); break ; // sinusoÏde carrée S&H + SEQ 
case 391 : NameTransfo = F(">duration");  break ; // durée en dixièmes de seconde pour un cycle + SEQ !
case 392 : NameTransfo = F(">which CC"); break ; // + SEQ
case 393 : NameTransfo = F(">which CH") ; break ;
case 394 : NameTransfo = F(">min value"); break ; // + SEQ
case 395 : NameTransfo = F(">max value"); break ;  // + SEQ
case 396 : NameTransfo = F(">step value"); break ; // TODO + SEQ
case 397 : NameTransfo = F(">machine specific"); break ; // TODO donne une valeur spéciale en fc de machine TX81Z MicroSampler Sherman2 

case 398 : NameTransfo = F("Drunk"); break ; // ON OFF sur layer
case 399 : NameTransfo = F(">min"); break ; 
case 400 : NameTransfo = F(">max"); break ;
case 401 : NameTransfo = F(">step"); break ; 

case 402 : NameTransfo = F("GammeN"); break ; 
// TODO il faut garder un game général et un mode général ( qui modife les 4 d'un coût ! )

case 403 : NameTransfo = F("Del Notes only"); break ; // 
case 404 : NameTransfo = F("Del CC only"); break ; // 
case 405 : NameTransfo = F("CC Reflet"); break ; // 
case 406 : NameTransfo = F("CC Mirror") ; break ; //  
case 407 : NameTransfo = F("CC Reduce") ; break ; //  va réduire le nb de points de CC dnas la layer 
case 408 : NameTransfo = F("CC Multiply"); break ; //  multiplication par 16ème de longueur de CC
case 409 : NameTransfo = F("CC Addition"); break ; // 
case 410 : NameTransfo = F("CC Grosser"); break ; // va amplifier les valeurs min et max, selon symétrie centrale à mi-course ( mid-value moyenne )

case 411 : NameTransfo = F("Arpeggio static"); break ; // va transformer les chords en arpèges 
// il faudrait aussi créer un UpdateArpeggio qui soit dynamique ! !!! 
case 412 :	NameTransfo = F("(sum max)Implicit ");	break;													// TODO va répartir jusqu'à ce que Sum des ² soit < à chiffre
case 413 :	NameTransfo = F("(1/0 ratio)Implicit ");	break;												 // TODO va répartir jusqu'à ce qu'on atteigne une certaine distribution entre 1 et 0
case 414 :	NameTransfo = F("(1 stop)Implicit ");	break;													// va répartir jusqu'à ce qu'on ait une distribution en 1 et 0
case 415 :	NameTransfo = F("(max n div)Implicit ");	break;												   //  va répartir jusqu'au minimum avec n div max
case 416 :	NameTransfo = F("(for n div)Implicit ");	break; // va répartir jusqu'au minimum avec n divisions précisément
case 417 :	NameTransfo = F("(max n div)Swingplicit ");	break; // répartition avec meilleur Swing possible !
case 418 : NameTransfo = F("(%swing best n)Swing  "); break ; // trouve la meilleure disposition de Swing avec n%
case 419 : NameTransfo = F("arpegiate"); break ; 
case 420 : NameTransfo = F("SORT pile"); break ; // swith notes EXPERIMENTAL 
case 421 : NameTransfo = F(">spanArp"); break ; // définit le span d'inclusion du Arp ( 30msec par défaut = chord )
case 422 : NameTransfo = F(">sort by"); break ; // définit ce qui fait la discrimination 

case 423 : NameTransfo = F(">CC or Transfo"); break ; // switch de LFO cc vers LFO Transfo value 

case 424 : NameTransfo = F("STEPZ"); break ; // step sequencer à la Electribe ! 
case 425 : NameTransfo = F(">nb Stepz FIX"); break ; 
case 426 : NameTransfo = F(">nb SubStpz"); break ; 
case 427 : NameTransfo = F(">CH step"); break ; 
case 428 : NameTransfo = F(">Vel step"); break ; // sera remplacé plus tard par slider
case 429 : NameTransfo = F(">stop mode"); break ; // définit si limité par stepz, substepz, ponctuel (10msec), ou rien 

// 4C17 ajout de valeurs exprimées en 32èmes 

case 430 : NameTransfo = F("Gabberism"); break ; // lance un gabberism sur le layer 
case 431 : NameTransfo = F(">levels"); break ; 
case 432 : NameTransfo = F(">divisions"); break ; 
case 433 : NameTransfo = F(">div ratio"); break ; 
case 434 : NameTransfo = F(">Repeat"); break ; 
case 435 : NameTransfo = F(">Rpt ratio"); break ; 

case 436 : NameTransfo = F(">stepz affiches") ; break ;
case 437 :	NameTransfo = F(">1B=t Stepz");	break;
case 438 : NameTransfo = F(">BPM FIX") ; break ;

case 439:	NameTransfo = F(">nb Stepz CHG");	break;
// case 440 :	NameTransfo = F(">1BPM = t Stepz (chgL)");	break;
case 440 : NameTransfo = F(">BPM CHG") ; break ;

case 441 : NameTransfo = F("Puntz"); break ; // peremt d'ajouter des notes sur l'affichage STEPZ

case 442 : NameTransfo = F(">1/n cutSize"); break ; // définit la taille du cut slice pour récupérer une tranche de mémoire
case 443 : NameTransfo = F("SliceMem"); break ; // récupère une tranche de mémoire 

// DENDE
case 444 : NameTransfo = F(">sec"); break ; // 
case 445 : NameTransfo = F(">msec"); break ; //  
case 446 : NameTransfo = F(">divide"); break ; // 
case 447 : NameTransfo = F(">drop"); break ; // 
case 448 : NameTransfo = F(">down"); break ; //  
case 449 : NameTransfo = F(">levels"); break ; // 
case 450 : NameTransfo = F(">branch"); break ; //  
case 451 : NameTransfo = F(">start"); break ; // 
case 452 : NameTransfo = F(">end"); break ; // 
case 453 : NameTransfo = F(">ratioDivide"); break ; // 
case 454 : NameTransfo = F(">stretch"); break ; // 
case 455 : NameTransfo = F(">pitch"); break ; //
case 456 : NameTransfo = F(">rnd happn"); break ; //
case 457 : NameTransfo = F(">overLoad"); break ; //
case 458 : NameTransfo = F(">retard"); break ; // 
case 459 : NameTransfo = F(">gamme"); break ; //
case 460 : NameTransfo = F(">mode"); break ;  // 
case 461 : NameTransfo = F(">degres"); break ; //
case 462 : NameTransfo = F(">VOID Expand"); break ; // 
case 463 : NameTransfo = F(">Arp1"); break ; // 
case 464 : NameTransfo = F(">Arp2"); break ; // 
case 465 : NameTransfo = F(">noteOff"); break ; // 
case 466 : NameTransfo = F("Dende"); break ;  //* 
case 467 : NameTransfo = F(">rnd appear"); break ; 
case 468 : NameTransfo = F(">BitMap"); break ; // pour Dende ici 

case 469 : NameTransfo = F("Add Slice"); break ; 

case 470 : NameTransfo = F("Kill CH"); break ;
case 471 : NameTransfo = F("Kill All-CH"); break ; // KILL ALL MINUS THAT CHANNEL ( exception )

// debug LFO cc
case 472 : NameTransfo = F("LFO Select"); break ;
case 473 : NameTransfo = F(">LFO data"); break ; 

case 474 : NameTransfo = F(">LFO Fidelity"); break ; 
case 475 : NameTransfo = F(">LFO mod Compteur"); break ;
case 476 : NameTransfo = F(">LFO mod WhereAt"); break ;

case 477 : NameTransfo = F("Replicate LFO"); break ; 
case 478 : NameTransfo = F(">LFO /32"); break ; 
case 479 : NameTransfo = F(">Machine"); break ;
case 480 : NameTransfo = F("Set CH-CC Machine specific"); break ;

case 481 : NameTransfo = F("Twoplets"); break ;
case 482 : NameTransfo = F(">space msec"); break ;
case 483 : NameTransfo = F(">space /32"); break ;
case 484 : NameTransfo = F("n-Plets"); break ;
case 485 : NameTransfo = F("TAP BPM"); break ; 
case 486 : NameTransfo = F(">/32"); break ; // pour Dende 
case 487 : NameTransfo = F("Center n"); break ; // amélioration de Center around C60 ;

case 488 : NameTransfo = F("transpo Degree") ; break ;  // Degree transposer
case 489 : NameTransfo = F(">typeCC/TF/Vd"); break ; // choix LFO type CC, Transfo ou Void 

case 490 : NameTransfo = F("Primer"); break ; // ajuste la longueur à des durées premières entre elles ( maximum arythmic )

case 491 : NameTransfo = F("Evt Gestion"); break ; // permet STOP ALL, START ALL, RND, INVERT des Events 

case 492 : NameTransfo = F("Variations"); break ; // entame le mode Variations 
case 493 : NameTransfo = F(">set Min notes"); break ; // set minimum notes for Variations + mode Minimum ON
case 494 : NameTransfo = F(">set ReduxMax notes"); break ; // set Redux max notes for variations + mode Minimum OFF, redux Max ON 
case 495 : NameTransfo = F(">%dissapear"); break ; // pourcentage de disparition de notes 
case 496 : NameTransfo = F(">mode"); break ; // choix du mode variation : 0 = dégressif 1 = constant 

case 497 : NameTransfo = F("Primer2"); break ; // ajuste la longueur à des durées premières entre elles ( maximum arythmic )

case 498 : NameTransfo = F("Evt Resume"); break ; // résume les données d'un Event 
case 499 : NameTransfo = F("Seq Resume"); break ; // résume les données d'une Séquence 
case 500 : NameTransfo = F("Evt Actif VIEW"); break ; // résume les Events actifs 
case 501 : NameTransfo = F("Seq Actif VIEW"); break ; // résume les Seq actives
// TODO distinguer actif et vide 

case 502 : NameTransfo = F("Fold"); break ; // fold de n /64èmes 
case 503 : NameTransfo = F("FoldShrt") ; break ; // fold de n / 64ème et shorten
case 504 : NameTransfo = F("unFold"); break ; // fold de n /64èmes 
case 505 : NameTransfo = F("unFoldLongr") ; break ; // fold de n / 64ème et shorten

case 506 : NameTransfo = F("batch mute"); break ; // batch expérimental mute puis unmute layers 

case 507 : NameTransfo = F("Karakt SAVE"); break ; // sauve les Karakt
case 508 : NameTransfo = F("Karakt LOAD"); break ; // charge les Karakt 

case 509 : NameTransfo = F("SV Karakt>>MEM"); break ; // sauve Karakt en mémoire // 340 
case 510 : NameTransfo = F("LD MEM>>Karakt"); break ; // charge les Karakt dela mémoire // 341
case 511 : NameTransfo = F("Del K_MEM"); break ; // efface une mémoire Karakt 

case 512 : NameTransfo = F("Lapidz"); break ; // mélange de Stepz et Variations 2C23 
case 513 : NameTransfo = F("keepStepzBUG"); break ; // préserve le bug de Stepz faisant double note avec effet bizarre vélocité 

case 514 : NameTransfo = F("verbose"); break ; // imprime sur le Serial la full pile 
case 515 : NameTransfo = F("CH mute"); break ; // mute un channel 
case 516 : NameTransfo = F("CH solo"); break ; // mute ALL but one channel 
case 517 : NameTransfo = F("CH all unmute"); break ; // unmute tous les channel 
case 518 : NameTransfo = F(">k/n cutSize"); break ; // spécifie le k du CutSize 

case 519 : NameTransfo = F("Gabberito"); break ; // nouveau Gabber style
case 520 : NameTransfo = F("G interval"); break ; // define Gabberiot interval

case 521 : NameTransfo = F("AccentMin"); break ; // Set Accent Min pour CH accent 
case 522 : NameTransfo = F("AccentMax"); break ; // set Accent Max pour CH accent 

case 523 : NameTransfo = F("save All K"); break ; // sauve Karakt actuel et les mémoires 
case 524 : NameTransfo = F("REC flux"); break ; // enregistre le flux de notes sortantes, venant de MIDI ou LAYERS

case 525 : NameTransfo = F("Gabberito 2"); break ; // version plus évoluée du Gabberito : d 
case 526 : NameTransfo = F(">K*(n rep)"); break ; // nombre de répétitions du groupe
case 527 : NameTransfo = F(">k*(N rep)"); break ; // nombre de répétitions dans un groupe
 case 528 : NameTransfo = F("flush"); break ; // flush la mémoire ONCE 
 
 case 529 : NameTransfo = F("GenTitle"); break ; // génère un titre
  case 530 : NameTransfo = F("Process"); break ; // génère un titre
 
 case 531 : NameTransfo = F("Permute"); break ; // permute les notes ( toujours même permut pour 1 note )
 
 case 532 : NameTransfo = F(">%Rnd Appear Kannon"); break ; // basé sur 387 pourcentage de random d'apparition du kannon 
case 533 : NameTransfo = F("CH destination"); break ; // soit idem ( kannon sur même CH), soit autre 
case 534 : NameTransfo = F("bitmap");	break; // Kannon bitmap pour éviter modulo et permettre plus de complexité

case 535 : NameTransfo = F("Tracker");	break; // Tracker ON/OFF + choix Seq

case 536 : NameTransfo = F("1st free Mem");	break;								// find first free mem
case 537 : NameTransfo = F("1st Kmem");	break; // find first Karakt free mem

case 538 : NameTransfo = F("quick Save");	break;								   // save to first free Mem ;
case 539 : NameTransfo = F("quick KSave");	break; // save to first free K Mem ;

case 540 : NameTransfo = F("Previous Load"); break ; // load Mem précédente
case 541 : NameTransfo = F("nb Repeat var idem"); break ; // repeater de variations idem 

case 542 : NameTransfo = F("Piano Roll"); break ; // va afficher une vue des "bonnes" notes pour gamme+mode 

case 543 : NameTransfo = F("Dialyse"); break ; // va séparer les layers actives en diverses mémoires 
case 544 : NameTransfo = F("Merge+"); break ; // va fusionner une Mem avec Layer actif

case 545 : NameTransfo = F("VerboseLayer"); break ; // va donner un verbose des layers - incroyable que ça arrive en pos 545 !

case 546 : NameTransfo = F("time n/-"); break ;
case 547 : NameTransfo = F("time -/n"); break ;
case 548 : NameTransfo = F("Polyz"); break ; // affichage polyrythimique du step séquenceur

//2C27 

case 549 : NameTransfo = F("midi info"); break ;
case 550 : NameTransfo = F("+-rnd"); break ; 
case 551 : NameTransfo = F("CC Transpose FloorCeiling"); break ;
case 552 : NameTransfo = F("verbose Serial"); break ; 

case 553 : NameTransfo = F("set CC number"); break ;
case 554 : NameTransfo = F("set CH number"); break ;
case 555 : NameTransfo = F("send CC Value"); break ;

case 556 : NameTransfo = F("MergeADD"); break ; 
case 557 : NameTransfo = F("Johnson"); break ; 

case 558 : NameTransfo = F("cut first note"); break ;
case 559 : NameTransfo = F("cut last note"); break ;

case 560 : NameTransfo = F("speed %"); break ; 

// 2C28

case 561 : NameTransfo = F("Spread Conway"); break ; 
case 562 : NameTransfo = F("Skip note"); break ; 
case 563 : NameTransfo = F("multiply ratio"); break ; 
case 564 : NameTransfo = F("AutoLiveFilter"); break ; 
case 565 : NameTransfo = F("gammeFilter"); break ;
case 566 : NameTransfo = F("modeFilter"); break ; 
case 567 : NameTransfo = F("type filtre"); break ; 

	case 568:		NameTransfo = F(">rnd appear");		break;   // pour délay
	case 569:		NameTransfo = F(">drop");		break; // pour Kannon 

case 570 : NameTransfo = F("Sustain Blocker"); break ; 
case 571 : NameTransfo = F("Proxy pitcher"); break ; 
case 572 : NameTransfo = F("Proxy pitch value"); break ; 

case 573 : NameTransfo = F("Decalagem"); break ;
case 574 : NameTransfo = F("% Decalagem"); break ;
case 575 : NameTransfo = F("max Decalagem"); break ; 

case 576 : NameTransfo = F("shutUP"); break ; 

case 577 : NameTransfo = F("postPone"); break ; 
case 578 : NameTransfo = F("buttonsCHK"); break ; 
case 579 : NameTransfo = F("Previous Karakt"); break ; 
case 580 : NameTransfo = F("BPM multi-bar"); break ; 

case 581 : NameTransfo = F("TracKarakt"); break ; 
case 582 : NameTransfo = F("Kb type"); break ; 

case 583 : NameTransfo = F("DMX send value"); break ; 
case 584 : NameTransfo = F("DMX auto generate"); break ; 
case 585 : NameTransfo = F("Ezy Mult"); break ; 
case 586 : NameTransfo = F("quick Trackerz STOP"); break ; 
case 587 : NameTransfo = F("VELcalagem"); break ;
case 588 : NameTransfo = F("Dec Swing"); break ; 
case 589 : NameTransfo = F("pinch DMX"); break ;
case 590 : NameTransfo = F("vertic slide CC"); break ;
case 591 : NameTransfo = F("horiz slide CC"); break ; 
case 592 : NameTransfo = F("vertic slide note"); break ;
case 593 : NameTransfo = F("horiz slide note"); break ; 
case 594 : NameTransfo = F("empty mult"); break ; 
case 595 : NameTransfo = F("gabbUltrino >> note picked by SEQ"); break ; 
case 596 : NameTransfo = F("set gUltrino SEQ"); break ; 
case 597 : NameTransfo = F("set CH CC"); break ; 
case 598 : NameTransfo = F("premonition"); break ;
case 599 : NameTransfo = F("prem vel"); break ;
case 600 : NameTransfo = F("prem rnd"); break ;
case 601 : NameTransfo = F("CC stepz rnd"); break ; 
case 602 : NameTransfo = F("choose CH CC"); break ; 

case 603 : NameTransfo = F("ACID line"); break ;
case 604 : NameTransfo = F(">Acid CH"); break; //	int acid_CH = 0; // CH pour la bassline Acid
case 605 : NameTransfo = F(">Acid Full ratio"); break; //	int acidNbFullNote = 10;
case 606 : NameTransfo = F(">Acid Empty ratio"); break; //	int acidNbEmptyNote = 7;
case 607 : NameTransfo = F(">Acid Short ratio"); break; //	int acidNbShortNote = 3;
case 608 : NameTransfo = F(">Acid triplet ratio"); break; //	int acidNbTripletNote = 2;
case 609 : NameTransfo = F(">Acid slide ratio"); break; //	int acidNbSlideNote = 5;
	// pour ces nombres, je n'emploie plsu des pourcentages, mais des ratio
	// par exemple si l'un vaut 2 et l'autre 3, la première apparaitra 2/(2+3) et l'autre 3/(2+3)
case 610 : NameTransfo = F(">Acid min pitch (24)"); break; //	int acidPitchMin = 24; // pour Bhringer TD3
case 611 : NameTransfo = F(">Acid max pitch (80)"); break; //	int acidPitchMax = 72; // pour Behringer TD3
case 612 : NameTransfo = F(">Acid min vel"); break; //	int acidVelocityMin = 80;
case 613 : NameTransfo = F(">Acid max vel"); break; //	int acidVelocityMax = 120;
case 614 : NameTransfo = F(">Acid divide (3)"); break;  // int acidNbDivide = 3 ; 
case 615 : NameTransfo = F("Transpose MIDI play"); break ; //

case 616 : NameTransfo = F(">lo VLV cut"); break ; 
case 617 : NameTransfo = F(">lo bring to"); break ;
case 618 : NameTransfo = F(">hi VLV cut"); break ; 
case 619 : NameTransfo = F(">hi bring to"); break ;
case 620 : NameTransfo = F("FLUX2 ON OFF"); break ; 

case 621 : NameTransfo = F(">CH pinch"); break ;
case 622:	NameTransfo = F(">CH size pinch"); break;
case 623:	NameTransfo = F(">CC pinch"); break;
case 624:	NameTransfo = F(">CC size pinch"); break;
case 625:	NameTransfo = F(">value pinch"); break;
case 626 : NameTransfo = F("PINCH multi"); break ;
case 627 : NameTransfo = F("Narayana"); break ;  // Lalacelle juillet 2022
case 628 : NameTransfo = F("-initial pitch"); break ;  //	int initialPitch = 64;						 // should be 64 : E
case 629 : NameTransfo = F("-scale"); break ;  //	const int tailleScale = 2;					 // should be 2
case 630 : NameTransfo = F("-note vs channel"); break ;  // 
case 631 : NameTransfo = F("-maturite"); break ;  //	int maturite = 4;							 // should be 4
case 632 : NameTransfo = F("-menopause"); break ;  //	int menopause = 1000;						 // should be infinite
case 633 : NameTransfo = F("-death age"); break ;  //	int mort = 1000;							 // should be infinite
case 634 : NameTransfo = F("-age first cow"); break ;  //	int ageInitialPremiereVache = 4;			 // should be 4
case 635 : NameTransfo = F("-children"); break ;  //	int taillePortee = 1;						 // nombre de veaux qui naissent en même temps : should be 1
case 636 : NameTransfo = F("-unexpec death"); break ;  //	int mortaliteFortuite = 0;					 // en % les veaux et vaches qui meurrent aléatoirement : should be 0

case 637 : NameTransfo = F("-bpm"); break ;  //	long Naranaya_BPM = 200;
case 638 : NameTransfo = F("-duree cow"); break ;  //	long dureeVache = (60 * 1000) / Naranaya_BPM;
case 639 : NameTransfo = F("-duree calf div"); break ;  //	long dureeVeau = dureeVache / 2; // should be 2

									 // TODO

	// Add Event from
	// Load Event from
	// gabberitos alternatifs
	// cc automat -> autre CH, autre CC
	// Game of life : evol, save memory
	// spread % mem
	// Merge AND OR XOR
	// option kill double note
	// Sort Chaos
	// Séquenceur de CC  : def CH, def CC, set value 

default:
	NameTransfo = F("none");
	};

	switch (mode)
	{
	case 0:
	{
		if (numeroteTransfo == true)
		{
			NameTransfo = NameTransfo + "." + String(number);
		}
	}
	break;
	case 1:
	{
		NameTransfo = String(LineCallingTransfo(number)) + "." + NameTransfo;

		NameTransfo += F(" (dev");
		NameTransfo += String(number);
		NameTransfo += ")";
	}
	break;
	case 2:
	{
		NameTransfo = String(number) + "." + NameTransfo;
	}
	break;
	// 2 est l'ancien 1
	default:
		break;
	}
}
