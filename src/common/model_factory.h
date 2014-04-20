//
// The MIT License (MIT)
//
// Copyright 2013-2014 The MilkCat Project Developers
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// model_factory.h --- Created at 2014-02-03
// libmilkcat.h --- Created at 2014-02-06
// model_factory.h --- Created at 2014-04-02
//

#include <string>
#include "common/milkcat_config.h"
#include "common/trie_tree.h"
#include "common/static_array.h"
#include "keyphrase/string_value.h"
#include "milkcat/hmm_model.h"
#include "milkcat/crf_model.h"
#include "milkcat/static_hashtable.h"
#include "utils/mutex.h"

#ifndef SRC_COMMON_MODEL_FACTORY_H_
#define SRC_COMMON_MODEL_FACTORY_H_

namespace milkcat {

// A factory class that can obtain any model data class needed by MilkCat
// in singleton mode. All the GetXX fucnctions are thread safe
class ModelFactory {
 public:
  explicit ModelFactory(const char *model_dir_path);
  ~ModelFactory();

  // Get the index for word which were used in unigram cost, bigram cost
  // hmm pos model and oov property
  const TrieTree *Index(Status *status);

  void SetUserDictionary(const char *path) { user_dictionary_path_ = path; }

  bool HasUserDictionary() const { return user_dictionary_path_.size() != 0; }

  const TrieTree *UserIndex(Status *status);
  const StaticArray<float> *UserCost(Status *status);

  const StaticArray<float> *UnigramCost(Status *status);
  const StaticHashTable<int64_t, float> *BigramCost(Status *status);

  // Get the CRF word segmenter model
  const CRFModel *CRFSegModel(Status *status);

  // Get the CRF word part-of-speech model
  const CRFModel *CRFPosModel(Status *status);

  // Get the HMM word part-of-speech model
  const HMMModel *HMMPosModel(Status *status);

  // Get the character's property in out-of-vocabulary word recognition
  const TrieTree *OOVProperty(Status *status);

  // Get the TFIDF model
  const StringValue<float> *IDFModel(Status *status);

  // Get the stopword list as an trietree index
  const TrieTree *Stopword(Status *status);

 private:
  std::string model_dir_path_;
  std::string user_dictionary_path_;
  utils::Mutex mutex;

  const TrieTree *unigram_index_;
  const TrieTree *user_index_;
  const StaticArray<float> *unigram_cost_;
  const StaticArray<float> *user_cost_;
  const StaticHashTable<int64_t, float> *bigram_cost_;
  const CRFModel *seg_model_;
  const CRFModel *crf_pos_model_;
  const HMMModel *hmm_pos_model_;
  const TrieTree *oov_property_;
  const StringValue<float> *idf_model_;
  const TrieTree *stopword_;

  // Load and set the user dictionary data specified by path
  void LoadUserDictionary(Status *status);
};

}  // namespace milkcat

#endif  // SRC_COMMON_MODEL_FACTORY_H_