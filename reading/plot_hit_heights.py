import ROOT
import sys

file_name = sys.argv[1]

rntuple_name_from_form = "root_rntuple__window_hit_finder:window_hit_finder__hit_reco"
field_name = "window_hits"

df = ROOT.RDataFrame(rntuple_name_from_form, file_name)
hist = df.Histo1D(("peak_adc_hist", "Peak ADC;ADC;Counts", 20, 55, 100), field_name + ".peak_adc_")

canvas = ROOT.TCanvas("peak_adc")
hist.Draw()
canvas.Print("peak_adc.png")
