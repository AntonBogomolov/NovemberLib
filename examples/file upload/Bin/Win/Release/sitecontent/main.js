function getInfo()
{
		var comUrl = '?command=getInfo';			
		var postData = 'userText=' + $("input[name='user_text']").val();		
						
		$.ajax({
		type: 'POST',
  		url: comUrl,
  		data: postData,
  		success: function(msg)
  		{   				
    		$("input[name='output']").val(msg);		
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