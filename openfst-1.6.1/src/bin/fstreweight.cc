// See www.openfst.org for extensive documentation on this weighted
// finite-state transducer library.
//
// Reweights an FST.

#include <cstring>

#include <memory>
#include <string>
#include <vector>

#include <fst/script/getters.h>
#include <fst/script/reweight.h>
#include <fst/script/text-io.h>

DEFINE_bool(to_final, false, "Push/reweight to final (vs. to initial) states");

int main(int argc, char **argv) {
  namespace s = fst::script;
  using fst::script::MutableFstClass;
  using fst::script::WeightClass;

  string usage = "Reweights an FST.\n\n  Usage: ";
  usage += argv[0];
  usage += " in.fst potential.txt [out.fst]\n";

  std::set_new_handler(FailedNewHandler);
  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc < 3 || argc > 4) {
    ShowUsage();
    return 1;
  }

  const string in_name = argv[1];
  const string potentials_name = argv[2];
  const string out_name = argc > 3 ? argv[3] : "";

  std::unique_ptr<MutableFstClass> fst(MutableFstClass::Read(in_name, true));
  if (!fst) return 1;

  std::vector<WeightClass> potential;
  if (!s::ReadPotentials(fst->WeightType(), potentials_name, &potential)) {
    return 1;
  }

  s::Reweight(fst.get(), potential, s::GetReweightType(FLAGS_to_final));

  fst->Write(out_name);

  return 0;
}
