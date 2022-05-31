(
SynthDef(\flute, {
    arg out = 0, scl = 0.2, freq = 440, ibreath = 0.09, ifeedbk1 = 0.4, ifeedbk2 = 0.4, gate = 1, amp = 0.2, pan = 0;
    var kenv1, kenv2, kenvibr, kvibr, sr, cr, block;
    var poly, signalOut, ifqc;
    var aflow1, asum1, asum2, afqc, atemp1, ax, apoly, asum3, avalue, atemp2, aflute1;
    var fdbckArray;

    sr = SampleRate.ir;
    cr = ControlRate.ir;
    block = cr.reciprocal;

    ifqc = freq;

    // noise envelope
    kenv1 = EnvGen.kr(Env.asr, gate);
    // overall envelope
    kenv2 = EnvGen.kr(Env.asr, gate);
    // vibrato envelope
    kenvibr = EnvGen.kr(Env.asr, gate);

    // create air flow and vibrato
    aflow1 = LFClipNoise.ar( sr, kenv1 );
    kvibr = SinOsc.ar( 5, 0, 0.1 * kenvibr );

    asum1 = ( ibreath * aflow1 ) + kenv1 + kvibr;
    afqc = ifqc.reciprocal - ( asum1/20000 ) - ( 9/sr ) + ( ifqc/12000000 ) - block;

    fdbckArray = LocalIn.ar( 1 );

    aflute1 = fdbckArray;
    asum2 = asum1 + ( aflute1 * ifeedbk1 );

    //ax = DelayL.ar( asum2, ifqc.reciprocal * 0.5, afqc * 0.5 );
    ax = DelayC.ar( asum2, ifqc.reciprocal - block * 0.5, afqc * 0.5 - ( asum1/ifqc/cr ) + 0.001 );

    apoly = ax - ( ax.cubed );
    asum3 = apoly + ( aflute1 * ifeedbk2 );
    avalue = LPF.ar( asum3, 2000 );

    aflute1 = DelayC.ar( avalue, ifqc.reciprocal - block, afqc );

    fdbckArray = [ aflute1 ];

    LocalOut.ar( fdbckArray );

    signalOut = avalue;
    signalOut.postln;
	signalOut = Pan2.ar(signalOut*kenv2, pan, amp/1.75);

	OffsetOut.ar(out, signalOut);
    }).add;

SynthDef(\sax, {
    arg out = 0, freq=440, amp = 0.2, gate = 1, pan = 0;
    var num = 16;
    var harms = Array.series(num, 1, 1) * Array.exprand(num, 0.995, 1.001);
    var snd = SinOsc.ar(freq * SinOsc.kr(Rand(2.0,5.0),0,Rand(0.001, 0.01),1) * harms, mul:Array.geom(num, 1, 0.63));
    snd = snd.sum;
    snd = BBandPass.ar(snd, freq * XLine.kr(0.1,4,0.01), 2);
    snd = snd * EnvGen.ar(Env.adsr(0.001, 0.2, 0.7, 0.2), gate);
	snd = Pan2.ar(snd, pan, amp);
    Out.ar(out, snd);
}).add;

SynthDef(\organ, {
    //subtractive tonewheel organ with cheap CPU usage
    arg out = 0, freq = 440, amp = 0.2, gate = 1, pan = 0;
    var sig, env, vibrato, atk = 0.001, sus = 1.0, rel = 0.0, tune = 0.29, tuneRate = 6.0, rq = 1;
    env = EnvGen.ar(Env.adsr, gate);
    vibrato = SinOsc.ar(tuneRate).range(freq, freq * (tune.midiratio));
    sig = LFPulse.ar(freq, 0, 0.5, 0.2) + LFPulse.ar(freq + vibrato, 0, 0.18);
    sig = RLPF.ar(sig, ((28.midiratio) * (freq)) , rq) ;
    sig = sig * env;
    sig = sig;
    sig = LeakDC.ar(sig);
    sig = Pan2.ar(sig, pan, amp);
    Out.ar(out, sig);
}).add;
)

~port.close

(
~nRec = NetAddr("127.0.0.1");
~sendingPort = 57109;
thisProcess.openUDPPort(~sendingPort);
~nSend = NetAddr.new("127.0.0.1", ~sendingPort);

~synthD = Dictionary.new;
~synthD.add(0 -> \flute);
~synthD.add(1 -> \sax);
~synthD.add(2 -> \organ);

~port = SerialPort.new("COM3",9600);
~charArray = [ ];
~synthType = \flute;
~octaveMult = 1;
~diesis = false;
~amp = 0.2;
~pan = 0;
~offset = 0;
~fluteNotes = Array.newClear(14);
~fluteFreqs = [130.81,146.83,164.81,174.61,196.00,220.00,246.94,138.59,155.56,174.61,185.00,207.65,233.08,261.63];

for(0, (~fluteNotes.size - 1), {
	arg i;
    ~fluteNotes[i] = Synth.new(~synthType, [freq: ~fluteFreqs[i], gate: 0.0]);
});

OSCFunc.newMatching({
	arg msg, time, addr, recvPort;
	try {
		~amp = msg[1];
	} {
		|error|
		"Error while changing volume".postln;
	}
}, '/gui/volume', ~nRec);

OSCFunc.newMatching({
	arg msg, time, addr, recvPort;
	try {
		~pan = msg[1];
	} {
		|error|
		"Error while changing pan".postln;
	}
}, '/gui/pan', ~nRec);

OSCFunc.newMatching({
	arg msg, time, addr, recvPort;
	try {
		~octaveMult = 2**(msg[1]);
	} {
		|error|
		~octaveMult = 1;
		"Error while changing octave".postln;
	}
}, '/gui/octave', ~nRec);

OSCFunc.newMatching({
	arg msg, time, addr, recvPort;
	~synthType = ~synthD.atFail(msg[1], {
		~synthType = \flute;
	});
	for(0, (~fluteNotes.size - 1), {
		arg i;
		~fluteNotes[i].set(\gate, 0);
		~fluteNotes[i].free;
		~fluteNotes[i] = nil;
		~fluteNotes[i] = Synth.new(~synthType, [freq: (~fluteFreqs[i]*~octaveMult), gate: 0.0]);
	});
}, '/gui/synth', ~nRec);

~controlSensors = Routine.new({
    var ascii, value;
    {
        ascii = ~port.read.asAscii;
        if(ascii == $f, {
            ~diesis = false;
			~offset = 0;
			for(0, (~fluteNotes.size - 1), {
				arg i;
				~fluteNotes[i].set(\gate, 0.0);
			});
			~nSend.sendMsg("/notes/noteOff", 7);
        });
        if(ascii == $t, {
            ~diesis = true;
			~offset = 7;
			for(0, (~fluteNotes.size - 1), {
				arg i;
				~fluteNotes[i].set(\gate, 0.0);
			});
			~nSend.sendMsg("/notes/noteOn", 7);
        });
        if(ascii == $a, {
            ~fluteNotes[0].set(\gate, 0.0);
            ~fluteNotes[1].set(\gate, 0.0);
			~fluteNotes[7].set(\gate, 0.0);
            ~fluteNotes[8].set(\gate, 0.0);
			~nSend.sendMsg("/notes/noteOff", 0);
			~nSend.sendMsg("/notes/noteOff", 1);
        });
        if(ascii == $b, {
            ~fluteNotes[2].set(\gate, 0.0);
            ~fluteNotes[3].set(\gate, 0.0);
			~fluteNotes[9].set(\gate, 0.0);
            ~fluteNotes[10].set(\gate, 0.0);
			~nSend.sendMsg("/notes/noteOff", 2);
			~nSend.sendMsg("/notes/noteOff", 3);
        });
        if(ascii == $c, {
            ~fluteNotes[4].set(\gate, 0.0);
            ~fluteNotes[5].set(\gate, 0.0);
			~fluteNotes[11].set(\gate, 0.0);
            ~fluteNotes[12].set(\gate, 0.0);
			~nSend.sendMsg("/notes/noteOff", 4);
			~nSend.sendMsg("/notes/noteOff", 5);
        });
        if(ascii == $d, {
            ~fluteNotes[6].set(\gate, 0.0);
			~fluteNotes[13].set(\gate, 0.0);
			~nSend.sendMsg("/notes/noteOff", 6);
        });
        if(ascii.isDecDigit, {
            ~charArray = ~charArray.add(ascii);
            value = ~charArray.collect(_.digit).convertDigits;
			~fluteNotes[value + ~offset].set(\gate, 1.0, \amp, ~amp, \pan, ~pan, \freq, (~fluteFreqs[value + ~offset]*~octaveMult));
            ~charArray = [ ];
			~nSend.sendMsg("/notes/noteOn", value);
        });

    }.loop;
}).play;
)