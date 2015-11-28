#!

ticvttc() {
    infocmp -L "$1" | sed -n "/^.*$2=\([^,]*\),.*/s//:$3=\1/p"
}

get_tc_list() {
	# Fn keys
    ticvttc "$1" key_f0 	'k0'
    ticvttc "$1" key_f1 	'k1'
    ticvttc "$1" key_f2 	'k2'
    ticvttc "$1" key_f3 	'k3'
    ticvttc "$1" key_f4 	'k4'
    ticvttc "$1" key_f5 	'k5'
    ticvttc "$1" key_f6 	'k6'
    ticvttc "$1" key_f7 	'k7'
    ticvttc "$1" key_f8 	'k8'
    ticvttc "$1" key_f9 	'k9'
    ticvttc "$1" key_f10 	'k;'
    ticvttc "$1" key_f11 	'F1'
    ticvttc "$1" key_f12 	'F2'
    ticvttc "$1" key_f13 	'F3'
    ticvttc "$1" key_f14 	'F4'
    ticvttc "$1" key_f15 	'F5'
    ticvttc "$1" key_f16 	'F6'
    ticvttc "$1" key_f17 	'F7'
    ticvttc "$1" key_f18 	'F8'
    ticvttc "$1" key_f19 	'F9'
    ticvttc "$1" key_f20 	'FA'
    ticvttc "$1" key_f21 	'FB'
    ticvttc "$1" key_f22 	'FC'
    ticvttc "$1" key_f23 	'FD'
    ticvttc "$1" key_f24 	'FE'
    ticvttc "$1" key_f25 	'FF'
    ticvttc "$1" key_f26 	'FG'
    ticvttc "$1" key_f27 	'FH'
    ticvttc "$1" key_f28 	'FI'
    ticvttc "$1" key_f29 	'FJ'
    ticvttc "$1" key_f30 	'FK'
    ticvttc "$1" key_f31 	'FL'
    ticvttc "$1" key_f32 	'FM'
    ticvttc "$1" key_f33 	'FN'
    ticvttc "$1" key_f34 	'FO'
    ticvttc "$1" key_f35 	'FP'
    ticvttc "$1" key_f36 	'FQ'
    ticvttc "$1" key_f37 	'FR'
    ticvttc "$1" key_f38 	'FS'
    ticvttc "$1" key_f39 	'FT'
    ticvttc "$1" key_f40 	'FU'
    ticvttc "$1" key_f41 	'FV'
    ticvttc "$1" key_f42 	'FW'
    ticvttc "$1" key_f43 	'FX'
    ticvttc "$1" key_f44 	'FY'
    ticvttc "$1" key_f45 	'FZ'
    ticvttc "$1" key_f46 	'Fa'
    ticvttc "$1" key_f47 	'Fb'
    ticvttc "$1" key_f48 	'Fc'
    ticvttc "$1" key_f49 	'Fd'
    ticvttc "$1" key_f50 	'Fe'
    ticvttc "$1" key_f51 	'Ff'
    ticvttc "$1" key_f52 	'Fg'
    ticvttc "$1" key_f53 	'Fh'
    ticvttc "$1" key_f54 	'Fi'
    ticvttc "$1" key_f55 	'Fj'
    ticvttc "$1" key_f56 	'Fk'
    ticvttc "$1" key_f57 	'Fl'
    ticvttc "$1" key_f58 	'Fm'
    ticvttc "$1" key_f59 	'Fn'
    ticvttc "$1" key_f60 	'Fo'
    ticvttc "$1" key_f61 	'Fp'
    ticvttc "$1" key_f62 	'Fq'
    ticvttc "$1" key_f63 	'Fr'

	# Shift-keys
    ticvttc "$1" key_sbeg 	'&9'
    ticvttc "$1" key_scancel 	'&0'
    ticvttc "$1" key_scommand 	'*1'
    ticvttc "$1" key_scopy 	'*2'
    ticvttc "$1" key_screate 	'*3'
    ticvttc "$1" key_sdc 	'*4'
    ticvttc "$1" key_sdl 	'*5'
    ticvttc "$1" key_send 	'*7'
    ticvttc "$1" key_seol 	'*8'
    ticvttc "$1" key_sexit 	'*9'
    ticvttc "$1" key_sf 	'kF'	# Scroll-forward == <Shift>+<Down>
    ticvttc "$1" key_sfind 	'*0'
    ticvttc "$1" key_shelp 	'#1'
    ticvttc "$1" key_shome 	'#2'
    ticvttc "$1" key_sic 	'#3'
    ticvttc "$1" key_sleft 	'#4'
    ticvttc "$1" key_smessage 	'%a'
    ticvttc "$1" key_smove 	'%b'
    ticvttc "$1" key_snext 	'%c'
    ticvttc "$1" key_soptions 	'%d'
    ticvttc "$1" key_sprevious 	'%e'
    ticvttc "$1" key_sprint 	'%f'
    ticvttc "$1" key_sr 	'kR'	# Scroll-backward == <Shift>+<Up>
    ticvttc "$1" key_sredo 	'%g'
    ticvttc "$1" key_sreplace 	'%h'
    ticvttc "$1" key_sright 	'%i'
    ticvttc "$1" key_srsume 	'%j'
    ticvttc "$1" key_ssave 	'!1'
    ticvttc "$1" key_ssuspend 	'!2'
    ticvttc "$1" key_stab 	'kT'
    ticvttc "$1" key_sundo 	'!3'
    ticvttc "$1" key_btab 	'kB'	# BackTab == <Shift>+<Tab>
}

get_tc_list "$1" | awk '{
	if(!getline s1) {
	    s1 = "";
	}
	if(!getline s2) {
	    s2 = "";
	}
	if(!getline s3) {
	    s3 = "";
	}
	if(!getline s4) {
	    s4 = "";
	}
	print "\t" $0 s1 s2 s3 s4 ":\\";
    }'

