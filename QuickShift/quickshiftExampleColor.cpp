#include <vl/quickshift.h>

#include <iostream>
#include <set>
#include <vector>

unsigned int ComputeLinearIndex(const unsigned int row, const unsigned int col, const unsigned int width, const unsigned int height, const unsigned int channel);

template <typename T>
std::vector<T> GetVectorFromArray(const T* array, const unsigned int size);

template<typename T>
void OutputMatrix(const T* array, const unsigned int width, const unsigned int height, const unsigned int channels = 1);

template<typename T>
void OutputMatrix(const std::vector<T>& v, const unsigned int width, const unsigned int height, const unsigned int channels = 1);

template<typename T>
void OutputArray(const T* array, const unsigned int length);

template<typename T>
void OutputVector(const std::vector<T>& v);

std::vector<int> GetLabelsFromParents(const std::vector<int>& parents);

bool VectorsIdential(const std::vector<int>& v1, const std::vector<int>& v2);

std::vector<int> SequentialLabels(const std::vector<int>& v);

int main()
{
  unsigned int width = 4;
  unsigned int height = 2;
  unsigned int channels = 3; // This is a color image
  // Create an image with the left half white and the right half black.
  unsigned int totalPixels = width*height;
  std::cout << "totalPixels: " << totalPixels << std::endl;
  
  vl_qs_type* image = new vl_qs_type[totalPixels*channels];
  
  vl_qs_type ratio = 10.0f;
  for(unsigned int channel = 0; channel < channels; ++channel)
    {
    for (unsigned int y = 0 ; y < height ; ++y)
      {
      for (unsigned int x = 0  ; x < width ; ++x) 
        {
        unsigned int linearIndex = ComputeLinearIndex(y, x, width, height, channel);
        std::cout << "linearIndex: " << linearIndex << std::endl;
        float noise = drand48()/10.0f;
        if(x <= 1)
          {
          //image[linearIndex] = 255;
          image[linearIndex] = 1.0 + noise;
          image[linearIndex] *= ratio;
          }
        else
          {
          image[linearIndex] = 0 + noise;
          }
        }
      }
    }

  // Create a new quick shift object
  
  VlQS* quickshift = vl_quickshift_new(image, height, width, channels);
  
  // Configure quick shift by setting the kernel size (vl_quickshift_set_kernel_size)
  // and the maximum gap (vl_quickshift_set_max_dist).
  // The latter is in principle not necessary, but useful to speedup processing.
  vl_qs_type sigma = 5.0;
  vl_quickshift_set_kernel_size(quickshift, sigma);
  vl_qs_type max_dist = 10.0;
  vl_quickshift_set_max_dist(quickshift, max_dist);

  //Process an image
  vl_quickshift_process(quickshift);
  

  //Retrieve the parents (These can be used to segment the image in superpixels.)
  int* parents = vl_quickshift_get_parents(quickshift);
  std::cout << "Parents:" << std::endl;
  OutputArray<int>(parents, totalPixels);
  
  std::vector<int> parentsVector = GetVectorFromArray(parents, totalPixels);
  
  std::vector<int> labels = GetLabelsFromParents(parentsVector);
  
  std::cout << "Labels vector:" << std::endl;
  OutputVector<int>(labels);
  
  std::vector<int> sequentialLabels = SequentialLabels(labels);
  std::cout << "Sequential labels vector:" << std::endl;
  OutputVector<int>(sequentialLabels);
  std::cout << "Sequential labels matrix:" << std::endl;
  OutputMatrix<int>(sequentialLabels, width, height);
  
  //Retrieve the distances (These can be used to segment the image in superpixels.)
  vl_qs_type* distances = vl_quickshift_get_dists(quickshift);
  // Pixels that are roots have distance=inf
  std::cout << "Distances array:" << std::endl;
  OutputArray<vl_qs_type>(distances, width*height);
  
  std::cout << "Distances matrix:" << std::endl;
  OutputMatrix<vl_qs_type>(distances, width, height);
  
  // Delete the quick shift object
  vl_quickshift_delete(quickshift);
  
  return 0;
}

template<typename T>
void OutputArray(const T* array, const unsigned int length)
{
  for(unsigned int i = 0; i < length; ++i)
    {
    std::cout << array[i] << " ";
    }
  std::cout << std::endl;
}

template<typename T>
void OutputVector(const std::vector<T>& v)
{
  for(unsigned int i = 0; i < v.size(); ++i)
    {
    std::cout << v[i] << " ";
    }
  std::cout << std::endl;
}


template<typename T>
void OutputMatrix(const std::vector<T>& v, const unsigned int width, const unsigned int height, const unsigned int channels = 1)
{
  for(unsigned int channel = 0; channel < channels; ++channel)
    {
    for(unsigned int row = 0; row < height; ++row)
      {
      for(unsigned int col = 0; col < width; ++col)
        {
        //std::cout << "Row: " << row << " col: " << col << std::endl;
        unsigned int linearIndex = ComputeLinearIndex(row, col, width, height, channel);
        //std::cout << "linearIndex: " << linearIndex << " value: " << array[linearIndex] << std::endl;
        std::cout << v[linearIndex] << " ";
        }
      std::cout << std::endl;
      }
    std::cout << std::endl << std::endl;
    }
}

template<typename T>
void OutputMatrix(const T* array, const unsigned int width, const unsigned int height, const unsigned int channels = 1)
{
  for(unsigned int channel = 0; channel < channels; ++channel)
    {
    for(unsigned int row = 0; row < height; ++row)
      {
      for(unsigned int col = 0; col < width; ++col)
        {
        //std::cout << "Row: " << row << " col: " << col << std::endl;
        unsigned int linearIndex = ComputeLinearIndex(row, col, width, height, channel);
        //std::cout << "linearIndex: " << linearIndex << " value: " << array[linearIndex] << std::endl;
        std::cout << array[linearIndex] << " ";
        }
      std::cout << std::endl;
      }
    std::cout << std::endl << std::endl;
    }
}

unsigned int ComputeLinearIndex(const unsigned int row, const unsigned int col, const unsigned int width, const unsigned int height, const unsigned int channel)
{
  return channel*width*height + row + height * col;
}

template <typename T>
std::vector<T> GetVectorFromArray(const T* array, const unsigned int size)
{
  std::vector<T> myVector(size);
  for(unsigned int i = 0; i < size; ++i)
    {
    myVector[i] = array[i];
    }
  return myVector;
}

std::vector<int> GetLabelsFromParents(const std::vector<int>& parents)
{
  // The values of the 'parents' array indicate the linear index of the pixel that is the parent of each pixel.
  // Compare to the LABELS output of [LABELS CLUSTERS] = VL_FLATMAP(MAP) in Matlab.

  std::vector<int> labels(parents.size(), 0);

  // Initialize by copying
  for(unsigned int i = 0; i < parents.size(); ++i)
    {
    labels[i] = parents[i];
    }
  
  std::vector<int> tempLabels(parents.size(), 0);
  std::vector<int> oldLabels(parents.size(), 0);
  
  while(!VectorsIdential(oldLabels, labels))
    {
    oldLabels = labels;
    for(unsigned int i = 0; i < parents.size(); ++i)
      {
      //std::cout << "Replacing " << labels[i] << " with " << labels[labels[i]] << std::endl;
      tempLabels[i] = labels[labels[i]]; // Replace my label with my parents label.
      }
    labels = tempLabels;
    }

  return labels;
}

bool VectorsIdential(const std::vector<int>& v1, const std::vector<int>& v2)
{
  if(v1.size() != v2.size())
    {
    std::cerr << "Vectors must be the same size!" << std::endl;
    exit(-1);
    }
  for(unsigned int i = 0; i < v1.size(); ++i)
    {
    if(v1[i] != v2[i])
      {
      return false;
      }
    }
  return true;
}

std::vector<int> SequentialLabels(const std::vector<int>& v)
{
  std::vector<int> sequentialLabels(v.size());
  std::set<int> uniqueLabels;
  for(unsigned int i = 0; i < v.size(); ++i)
    {
    uniqueLabels.insert(v[i]);
    }

  // Set old values to new sequential labels
  unsigned int sequentialLabelId = 0;
  for(typename std::set<int>::iterator it1 = uniqueLabels.begin(); it1 != uniqueLabels.end(); it1++)
    {
    
    for(unsigned int i = 0; i < v.size(); ++i)
      {
      // We check the input image because if we change pixels in the output image and then search it later, we could accidentially write incorrect values.
      if(v[i] == *it1)
        {
        sequentialLabels[i] = sequentialLabelId;
        }
      }
    sequentialLabelId++;
    }
  return sequentialLabels;
}
