function delFile(fileId)
{
		var comUrl = '?command=delfile';			
		var userId = $("input[name='user_id']").val();
		var postData = 'id=' + fileId + '&user_id=' + userId;	
						
		$.ajax({
		type: 'POST',
  		url: comUrl,
  		data: postData,
  		success: function(msg)
  		{   				
    		location.reload();		
  		},
		error: function () 
  		{
  				
  		}
		});		
}

 $(document).ready(
	function() 
	{
	
	}
);