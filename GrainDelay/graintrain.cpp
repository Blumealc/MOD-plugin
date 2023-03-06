#include "graintrain.h"

namespace graintrain {

/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2022 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing (at) cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	Data m_inBuffer_6;
	Data m_data_param_7;
	Delay m_del_inR_8;
	Delay m_del_inL_9;
	int __loopcount;
	int __exception;
	int vectorsize;
	t_sample m_his_mix_4;
	t_sample m_grainSize_15;
	t_sample m_width_14;
	t_sample m_pitch_13;
	t_sample m_interval_16;
	t_sample __m_carry_33;
	t_sample __m_count_31;
	t_sample m_his_gain_3;
	t_sample m_spray_12;
	t_sample m_drywet_10;
	t_sample m_counter_2;
	t_sample m_his_index_5;
	t_sample m_reverse_11;
	t_sample m_rcount_1;
	t_sample __m_slide_81;
	t_sample samplerate;
	t_sample __m_slide_84;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_rcount_1 = ((int)0);
		m_counter_2 = ((int)0);
		m_his_gain_3 = ((int)0);
		m_his_mix_4 = ((int)0);
		m_his_index_5 = ((int)0);
		m_inBuffer_6.reset("inBuffer", samplerate, ((int)2));
		m_data_param_7.reset("data_param", ((int)6), ((int)100));
		m_del_inR_8.reset("m_del_inR_8", (samplerate * ((int)20)));
		m_del_inL_9.reset("m_del_inL_9", (samplerate * ((int)20)));
		m_drywet_10 = ((int)0);
		m_reverse_11 = ((int)0);
		m_spray_12 = ((int)0);
		m_pitch_13 = ((int)0);
		m_width_14 = ((t_sample)0.5);
		m_grainSize_15 = ((int)100);
		m_interval_16 = ((int)100);
		__m_count_31 = 0;
		__m_carry_33 = 0;
		__m_slide_81 = 0;
		__m_slide_84 = 0;
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) {
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) {
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) {
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		int inBuffer_dim = m_inBuffer_6.dim;
		int inBuffer_channels = m_inBuffer_6.channels;
		t_sample count_max_34 = (m_interval_16 * (samplerate * 0.001));
		__loopcount = (__n * GENLIB_LOOPCOUNT_BAIL);
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			int index_trunc_17 = fixnan(floor(m_counter_2));
			bool index_ignore_18 = ((index_trunc_17 >= inBuffer_dim) || (index_trunc_17 < 0));
			if ((!index_ignore_18)) {
				m_inBuffer_6.write(in1, index_trunc_17, 0);
				
			};
			bool chan_ignore_19 = ((((int)1) < 0) || (((int)1) >= inBuffer_channels));
			int index_trunc_20 = fixnan(floor(m_counter_2));
			bool index_ignore_21 = ((index_trunc_20 >= inBuffer_dim) || (index_trunc_20 < 0));
			if ((!(chan_ignore_19 || index_ignore_21))) {
				m_inBuffer_6.write(in2, index_trunc_20, ((int)1));
				
			};
			int index_trunc_22 = fixnan(floor(m_rcount_1));
			bool index_ignore_23 = ((index_trunc_22 >= inBuffer_dim) || (index_trunc_22 < 0));
			// samples inBuffer channel 1;
			int chan_24 = ((int)0);
			bool chan_ignore_25 = ((chan_24 < 0) || (chan_24 >= inBuffer_channels));
			t_sample peek_415 = ((chan_ignore_25 || index_ignore_23) ? 0 : m_inBuffer_6.read(index_trunc_22, chan_24));
			t_sample peek_416 = m_rcount_1;
			t_sample smpL = (revWin_d(m_rcount_1) * peek_415);
			int index_trunc_27 = fixnan(floor(m_rcount_1));
			bool index_ignore_28 = ((index_trunc_27 >= inBuffer_dim) || (index_trunc_27 < 0));
			// samples inBuffer channel 1;
			int chan_29 = ((int)1);
			bool chan_ignore_30 = ((chan_29 < 0) || (chan_29 >= inBuffer_channels));
			t_sample peek_417 = ((chan_ignore_30 || index_ignore_28) ? 0 : m_inBuffer_6.read(index_trunc_27, chan_29));
			t_sample peek_418 = m_rcount_1;
			t_sample smpR = (revWin_d(m_rcount_1) * peek_417);
			m_del_inL_9.write(smpL);
			m_del_inR_8.write(smpR);
			__m_count_31 = (((int)0) ? 0 : (fixdenorm(__m_count_31 + ((int)1))));
			int carry_32 = 0;
			if ((((int)0) != 0)) {
				__m_count_31 = 0;
				__m_carry_33 = 0;
				
			} else if (((count_max_34 > 0) && (__m_count_31 >= count_max_34))) {
				int wraps_35 = (__m_count_31 / count_max_34);
				__m_carry_33 = (__m_carry_33 + wraps_35);
				__m_count_31 = (__m_count_31 - (wraps_35 * count_max_34));
				carry_32 = 1;
				
			};
			int mCout = __m_count_31;
			if ((mCout == ((int)1))) {
				int data_param_dim = m_data_param_7.dim;
				int data_param_channels = m_data_param_7.channels;
				// samples data_param channel 1;
				int chan_36 = m_his_index_5;
				bool chan_ignore_37 = ((chan_36 < 0) || (chan_36 >= data_param_channels));
				t_sample peek_419 = (chan_ignore_37 ? 0 : m_data_param_7.read(0, chan_36));
				t_sample peek_420 = ((int)0);
				if ((peek_419 == ((int)0))) {
					int channel_trunc_38 = fixnan(floor(m_his_index_5));
					bool chan_ignore_39 = ((channel_trunc_38 < 0) || (channel_trunc_38 >= data_param_channels));
					if ((!chan_ignore_39)) {
						m_data_param_7.write(((int)1), 0, channel_trunc_38);
						
					};
					int channel_trunc_40 = fixnan(floor(m_his_index_5));
					bool chan_ignore_41 = ((channel_trunc_40 < 0) || (channel_trunc_40 >= data_param_channels));
					bool index_ignore_42 = (((int)1) >= data_param_dim);
					if ((!(chan_ignore_41 || index_ignore_42))) {
						m_data_param_7.write((m_grainSize_15 * (samplerate * 0.001)), ((int)1), channel_trunc_40);
						
					};
					t_sample sizeScaled = (m_grainSize_15 * (samplerate * 0.001));
					t_sample widthRand = ((noise() * ((t_sample)0.5)) * m_width_14);
					int channel_trunc_43 = fixnan(floor(m_his_index_5));
					bool chan_ignore_44 = ((channel_trunc_43 < 0) || (channel_trunc_43 >= data_param_channels));
					bool index_ignore_45 = (((int)3) >= data_param_dim);
					if ((!(chan_ignore_44 || index_ignore_45))) {
						m_data_param_7.write((((t_sample)0.5) + widthRand), ((int)3), channel_trunc_43);
						
					};
					int channel_trunc_46 = fixnan(floor(m_his_index_5));
					bool chan_ignore_47 = ((channel_trunc_46 < 0) || (channel_trunc_46 >= data_param_channels));
					bool index_ignore_48 = (((int)4) >= data_param_dim);
					if ((!(chan_ignore_47 || index_ignore_48))) {
						m_data_param_7.write((((t_sample)0.5) - widthRand), ((int)4), channel_trunc_46);
						
					};
					t_sample pitchScaled = (safepow(((int)2), (m_pitch_13 * ((t_sample)0.083333333333333))) - ((int)1));
					int channel_trunc_49 = fixnan(floor(m_his_index_5));
					bool chan_ignore_50 = ((channel_trunc_49 < 0) || (channel_trunc_49 >= data_param_channels));
					bool index_ignore_51 = (((int)5) >= data_param_dim);
					if ((!(chan_ignore_50 || index_ignore_51))) {
						m_data_param_7.write(pitchScaled, ((int)5), channel_trunc_49);
						
					};
					t_sample posScaled = (((samplerate * fabs(noise())) * m_spray_12) * ((t_sample)0.5));
					int channel_trunc_52 = fixnan(floor(m_his_index_5));
					bool chan_ignore_53 = ((channel_trunc_52 < 0) || (channel_trunc_52 >= data_param_channels));
					bool index_ignore_54 = (((int)2) >= data_param_dim);
					if ((!(chan_ignore_53 || index_ignore_54))) {
						t_sample maxa_55 = (sizeScaled * pitchScaled);
						m_data_param_7.write((posScaled + ((maxa_55 < ((int)0)) ? ((int)0) : maxa_55)), ((int)2), channel_trunc_52);
						
					};
					m_his_index_5 = wrap((m_his_index_5 + ((int)1)), ((int)0), ((int)100));
					
				};
				
			};
			t_sample outL = ((int)0);
			t_sample outR = ((int)0);
			t_sample totalWin = ((int)0);
			// for loop initializer;
			int i = ((int)0);
			// for loop condition;
			// abort processing if an infinite loop is suspected;
			if (((__loopcount--) <= 0)) {
				__exception = GENLIB_ERR_LOOP_OVERFLOW;
				break ;
				
			};
			while ((i < ((int)100))) {
				// abort processing if an infinite loop is suspected;
				if (((__loopcount--) <= 0)) {
					__exception = GENLIB_ERR_LOOP_OVERFLOW;
					break ;
					
				};
				int data_param_dim = m_data_param_7.dim;
				int data_param_channels = m_data_param_7.channels;
				// samples data_param channel 1;
				int chan_56 = i;
				bool chan_ignore_57 = ((chan_56 < 0) || (chan_56 >= data_param_channels));
				t_sample pCout = (chan_ignore_57 ? 0 : m_data_param_7.read(0, chan_56));
				if ((pCout != ((int)0))) {
					bool index_ignore_58 = (((int)1) >= data_param_dim);
					// samples data_param channel 1;
					int chan_59 = i;
					bool chan_ignore_60 = ((chan_59 < 0) || (chan_59 >= data_param_channels));
					t_sample s = ((chan_ignore_60 || index_ignore_58) ? 0 : m_data_param_7.read(((int)1), chan_59));
					if ((pCout < s)) {
						t_sample pCoutScaled = safediv(pCout, s);
						t_sample win = (((t_sample)0.5) * (sin((((t_sample)4.7123889803847) + (pCoutScaled * ((t_sample)6.2831853071796)))) + ((int)1)));
						bool index_ignore_61 = (((int)2) >= data_param_dim);
						// samples data_param channel 1;
						int chan_62 = i;
						bool chan_ignore_63 = ((chan_62 < 0) || (chan_62 >= data_param_channels));
						t_sample peek_421 = ((chan_ignore_63 || index_ignore_61) ? 0 : m_data_param_7.read(((int)2), chan_62));
						t_sample peek_422 = ((int)2);
						bool index_ignore_64 = (((int)5) >= data_param_dim);
						// samples data_param channel 1;
						int chan_65 = i;
						bool chan_ignore_66 = ((chan_65 < 0) || (chan_65 >= data_param_channels));
						t_sample peek_423 = ((chan_ignore_66 || index_ignore_64) ? 0 : m_data_param_7.read(((int)5), chan_65));
						t_sample peek_424 = ((int)5);
						t_sample outputL = (m_del_inL_9.read_linear((peek_421 - (pCout * peek_423))) * win);
						bool index_ignore_67 = (((int)2) >= data_param_dim);
						// samples data_param channel 1;
						int chan_68 = i;
						bool chan_ignore_69 = ((chan_68 < 0) || (chan_68 >= data_param_channels));
						t_sample peek_425 = ((chan_ignore_69 || index_ignore_67) ? 0 : m_data_param_7.read(((int)2), chan_68));
						t_sample peek_426 = ((int)2);
						bool index_ignore_70 = (((int)5) >= data_param_dim);
						// samples data_param channel 1;
						int chan_71 = i;
						bool chan_ignore_72 = ((chan_71 < 0) || (chan_71 >= data_param_channels));
						t_sample peek_427 = ((chan_ignore_72 || index_ignore_70) ? 0 : m_data_param_7.read(((int)5), chan_71));
						t_sample peek_428 = ((int)5);
						t_sample outputR = (m_del_inL_9.read_linear((peek_425 - (pCout * peek_427))) * win);
						bool index_ignore_73 = (((int)3) >= data_param_dim);
						// samples data_param channel 1;
						int chan_74 = i;
						bool chan_ignore_75 = ((chan_74 < 0) || (chan_74 >= data_param_channels));
						t_sample peek_429 = ((chan_ignore_75 || index_ignore_73) ? 0 : m_data_param_7.read(((int)3), chan_74));
						t_sample peek_430 = ((int)3);
						outL = (outL + (outputL * peek_429));
						bool index_ignore_76 = (((int)4) >= data_param_dim);
						// samples data_param channel 1;
						int chan_77 = i;
						bool chan_ignore_78 = ((chan_77 < 0) || (chan_77 >= data_param_channels));
						t_sample peek_431 = ((chan_ignore_78 || index_ignore_76) ? 0 : m_data_param_7.read(((int)4), chan_77));
						t_sample peek_432 = ((int)4);
						outR = (outR + (outputR * peek_431));
						bool chan_ignore_79 = ((i < 0) || (i >= data_param_channels));
						if ((!chan_ignore_79)) {
							m_data_param_7.write((pCout + ((int)1)), 0, i);
							
						};
						totalWin = (totalWin + win);
						
					} else {
						bool chan_ignore_80 = ((i < 0) || (i >= data_param_channels));
						if ((!chan_ignore_80)) {
							m_data_param_7.write(((int)0), 0, i);
							
						};
						
					};
					
				};
				// for loop increment;
				i = (i + ((int)1));
				
			};
			t_sample normalise = safepow(safediv(((int)1), ((totalWin < ((int)1)) ? ((int)1) : totalWin)), ((t_sample)0.3));
			t_sample left_sig = (outL * normalise);
			t_sample right_sig = (outR * normalise);
			t_sample iup_82 = (1 / maximum(1, abs(((int)128))));
			t_sample idown_83 = (1 / maximum(1, abs(((int)128))));
			__m_slide_81 = fixdenorm((__m_slide_81 + (((m_drywet_10 > __m_slide_81) ? iup_82 : idown_83) * (m_drywet_10 - __m_slide_81))));
			t_sample slide_453 = __m_slide_81;
			t_sample mix_452 = (in1 + (slide_453 * (left_sig - in1)));
			t_sample iup_85 = (1 / maximum(1, abs(((int)128))));
			t_sample idown_86 = (1 / maximum(1, abs(((int)128))));
			__m_slide_84 = fixdenorm((__m_slide_84 + (((m_drywet_10 > __m_slide_84) ? iup_85 : idown_86) * (m_drywet_10 - __m_slide_84))));
			t_sample slide_455 = __m_slide_84;
			t_sample mix_454 = (in2 + (slide_455 * (right_sig - in2)));
			m_counter_2 = safemod((m_counter_2 + ((int)1)), samplerate);
			if ((m_reverse_11 == ((int)1))) {
				m_rcount_1 = (samplerate - m_counter_2);
				
			} else {
				m_rcount_1 = m_counter_2;
				
			};
			t_sample out1 = mix_452;
			t_sample out2 = mix_454;
			m_del_inR_8.step();
			m_del_inL_9.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_inBuffer(void * _value) {
		m_inBuffer_6.setbuffer(_value);
	};
	inline void set_data_param(void * _value) {
		m_data_param_7.setbuffer(_value);
	};
	inline void set_drywet(t_param _value) {
		m_drywet_10 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_reverse(t_param _value) {
		m_reverse_11 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_spray(t_param _value) {
		m_spray_12 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_pitch(t_param _value) {
		m_pitch_13 = (_value < -24 ? -24 : (_value > 24 ? 24 : _value));
	};
	inline void set_width(t_param _value) {
		m_width_14 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_grainSize(t_param _value) {
		m_grainSize_15 = (_value < 0.01 ? 0.01 : (_value > 20000 ? 20000 : _value));
	};
	inline void set_interval(t_param _value) {
		m_interval_16 = (_value < 0.1 ? 0.1 : (_value > 10000 ? 10000 : _value));
	};
	inline t_sample revWin_d(t_sample idx) {
		t_sample cc = (safediv(idx, samplerate) + ((t_sample)0.5));
		t_sample v_26 = (cos(((cc * ((t_sample)3.1415926535898)) * ((t_sample)0.5))) * ((int)2));
		t_sample win = tanh(((v_26 <= ((int)0)) ? ((int)0) : ((v_26 >= ((int)1)) ? ((int)1) : v_26)));
		return win;
		
	};
	
} State;


///
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs

int gen_kernel_numins = 2;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 9; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1", "in2" };
const char *gen_kernel_outnames[] = { "out1", "out2" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_data_param(ref); break;
		case 1: self->set_drywet(value); break;
		case 2: self->set_grainSize(value); break;
		case 3: self->set_inBuffer(ref); break;
		case 4: self->set_interval(value); break;
		case 5: self->set_pitch(value); break;
		case 6: self->set_reverse(value); break;
		case 7: self->set_spray(value); break;
		case 8: self->set_width(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		
		case 1: *value = self->m_drywet_10; break;
		case 2: *value = self->m_grainSize_15; break;
		
		case 4: *value = self->m_interval_16; break;
		case 5: *value = self->m_pitch_13; break;
		case 6: *value = self->m_reverse_11; break;
		case 7: *value = self->m_spray_12; break;
		case 8: *value = self->m_width_14; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(9 * sizeof(ParamInfo));
	self->__commonstate.numparams = 9;
	// initialize parameter 0 ("m_data_param_7")
	pi = self->__commonstate.params + 0;
	pi->name = "data_param";
	pi->paramtype = GENLIB_PARAMTYPE_SYM;
	pi->defaultvalue = 0.;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = false;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_drywet_10")
	pi = self->__commonstate.params + 1;
	pi->name = "drywet";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_drywet_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_grainSize_15")
	pi = self->__commonstate.params + 2;
	pi->name = "grainSize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_grainSize_15;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 20000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_inBuffer_6")
	pi = self->__commonstate.params + 3;
	pi->name = "inBuffer";
	pi->paramtype = GENLIB_PARAMTYPE_SYM;
	pi->defaultvalue = 0.;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = false;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_interval_16")
	pi = self->__commonstate.params + 4;
	pi->name = "interval";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_interval_16;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_pitch_13")
	pi = self->__commonstate.params + 5;
	pi->name = "pitch";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_pitch_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -24;
	pi->outputmax = 24;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_reverse_11")
	pi = self->__commonstate.params + 6;
	pi->name = "reverse";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_reverse_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_spray_12")
	pi = self->__commonstate.params + 7;
	pi->name = "spray";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spray_12;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 8 ("m_width_14")
	pi = self->__commonstate.params + 8;
	pi->name = "width";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_width_14;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // graintrain::
