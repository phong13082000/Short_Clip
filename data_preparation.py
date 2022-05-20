#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np


# In[8]:


mu, sigma = 6, 3


# In[9]:


user_trace = np.random.normal(mu, sigma, 30)
np.sum(user_trace)


# In[10]:


user_trace

