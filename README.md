# Multi-Stream-File-Transfer
A multistream file transfer program written in cpp using socket interface.<br>
File can be transfered from one computer to another over the network using socket interface.<br>
At the senders end file is divided into equal fragments and all fragments are sent in parallel. <br>
At the receivers end every received fragment is stored in a file and after the file transfer is complete all the fragments are merged into single file. <br>
All the fragments have .temp extension and can be found in hidden directory " .filetransfer ". <br>
Fragments are not deleted for demonstration purpose. <br>

