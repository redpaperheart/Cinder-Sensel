Cinder-Sensel
===================
Cinder block for integrating Sensel Morph SDK

This Block does not cover all functionality that is provided by the Sensel SDK, but it helps setting up all header and libraries paths.

##### Adding this block to Cinder
This block was built with version 0.9.1, so 0.9.0 and up should work. This block is also is currently tested on Windows OS only.

* First get a Sensel Morph device. [Sensel Morph]( https://sensel.com/pages/the-sensel-morph )
* Second download the Sensel Driver. [Sensel Driver](https://www.flir.com/products/spinnaker-sdk/)
  - This block is configured to work with the default Sensel driver install location `C:\Program Files\Sensel` on windows and `/usr/local/include`, `/usr/local/lib` on mac.
  - If you install the SDK at a different path you'll need to update all include paths

A sample project is included with the block. CinderSenselSample is a very simple barebones example that draws the sensel's force data.

##### Tips and Gotchas
- This block was tested with at most two Sensel Morph devices running simultaneously.


License
-------
Copyright (c) 2019, Red Paper Heart, All rights reserved. To contact Red Paper Heart, email hello@redpaperheart.com or tweet @redpaperhearts

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
