import ROOT
import sys

file_name = sys.argv[1]
spill = sys.argv[2]
apa = sys.argv[3]
channel = sys.argv[4]

rntuple_name_from_form = "root_rntuple__wires_provider:simulated_wires__sim"
waveform_field_name_from_form = "sim._0.waveform_"
n_channels = 1024
n_apas = 24

reader = ROOT.RNTupleReader.Open(rntuple_name_from_form, file_name)
waveforms_view = reader.GetView["std::vector<uint16_t>"](waveform_field_name_from_form)
waveforms = waveforms_view(int(channel) + int(apa) * n_channels + int(spill) * n_channels * n_apas)

canvas = ROOT.TCanvas("waveform", "waveform viewer")
hist = ROOT.TH1D("waveform_hist", "Waveform;Time Tick;ADC Ticks", waveforms.size(), 0, waveforms.size()-1)
for channel, value in enumerate(waveforms):
  hist.SetBinContent(channel, value)
hist.Draw()
canvas.Print("waveform_spill_" + str(spill) + "_apa_" + str(apa) + ".png")
